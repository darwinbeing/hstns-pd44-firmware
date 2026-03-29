#!/usr/bin/env python3
import csv
from pathlib import Path
import sys


Q15_SHIFT = 15

COMP_N1 = int(-1.95197 * (1 << Q15_SHIFT))
COMP_N2 = int(2.424896 * (1 << Q15_SHIFT))
COMP_N3 = int(-0.69126 * (1 << Q15_SHIFT))
COMP_D2 = int(0.25742 * (1 << Q15_SHIFT))
COMP_D3 = int(-0.00742 * (1 << Q15_SHIFT))


def clamp(value, lo, hi):
    return max(lo, min(hi, value))


def stimulus(step, llc_period_cmd, plant_state):
    settle_dither = (-3, -2, -1, 0, 1, 2, 1, 0)
    cmd = clamp(llc_period_cmd, 2600, 24000)
    plant_target = 3119 - ((cmd - 3500) // 4)
    plant_target = clamp(plant_target, 1200, 3900)
    plant_state = plant_state + ((plant_target - plant_state) // 4)
    plant_state = clamp(plant_state + settle_dither[step & 7], 1200, 3700)
    target = plant_state
    an0 = (target >> 2) + 2
    an2 = (target >> 2) - 2
    return an0, an2, ((an0 + an2) << 1), plant_state


def run_model(steps):
    rows = []

    vout_setpoint = 0x0C4D
    llc_period_cmd = 0x5DC0
    comp_y_n1 = 0x5DC0
    comp_y_n2 = 0x5DC0
    prev_error = 0
    prev_prev_error = 0
    droop_kff = 0x0400
    plant_state = 3700

    for step in range(steps):
        an0, an2, vout_sum, plant_state = stimulus(step, llc_period_cmd, plant_state)

        error = vout_setpoint - vout_sum
        delta = error - prev_error
        if delta > 100:
            error = prev_error + 50
        elif delta < -100:
            error = prev_error - 50
        error = clamp(error, -100, 100)

        dac_sp = ((llc_period_cmd * 0x44D) >> 15) - 0x4A
        dac_sp = clamp(dac_sp, 44, 128)
        kff_scaled = (dac_sp * droop_kff) >> 10

        denom = COMP_D2 * comp_y_n1 + COMP_D3 * comp_y_n2
        numer = error * COMP_N1 + prev_error * COMP_N2 + prev_prev_error * COMP_N3
        combined = denom + ((kff_scaled * numer) >> 7)
        comp_y_out = combined >> 13
        comp_y_out = clamp(comp_y_out, 2600, 25000)

        llc_period_cmd = comp_y_out
        if llc_period_cmd <= 0x0D47:
            llc_period_cmd = 0x0D48

        ptper_cmd = int((0x00B3FB00 // max(llc_period_cmd, 1)) >> 1) - 8

        rows.append(
            {
                "step": step,
                "an0": an0,
                "an2": an2,
                "vout_sum": vout_sum,
                "vout_setpoint": vout_setpoint,
                "voltage_error": error,
                "comp_y_out": comp_y_out,
                "llc_period_cmd": llc_period_cmd,
                "ptper_cmd": ptper_cmd,
            }
        )

        comp_y_n2 = comp_y_n1
        comp_y_n1 = comp_y_out
        prev_prev_error = prev_error
        prev_error = error

    return rows


def write_csv(rows, path):
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=[
                "step",
                "an0",
                "an2",
                "vout_sum",
                "vout_setpoint",
                "voltage_error",
                "comp_y_out",
                "llc_period_cmd",
                "ptper_cmd",
            ],
        )
        writer.writeheader()
        writer.writerows(rows)


def write_svg(rows, path):
    width = 1280
    height = 820
    left = 80
    panel_w = 930
    top1 = 110
    top2 = 420
    panel_h1 = 220
    panel_h2 = 260

    def x_of(i):
        if len(rows) <= 1:
            return left
        return left + i * panel_w / (len(rows) - 1)

    def y_of(value, lo, hi, top, h):
        if hi <= lo:
            return top + h / 2
        return top + h - ((value - lo) * h / (hi - lo))

    def panel(series, top, h, title):
        vals = [row[key] for key, _, _ in series for row in rows]
        lo = min(vals)
        hi = max(vals)
        pad = max(1, int((hi - lo) * 0.08) if hi > lo else 1)
        lo -= pad
        hi += pad
        out = [f'<rect x="{left}" y="{top}" width="{panel_w}" height="{h}" fill="#ffffff" stroke="#d0d7de"/>']
        out.append(f'<text x="{left}" y="{top - 14}" font-size="16" font-family="Helvetica, Arial, sans-serif" fill="#111827">{title}</text>')
        for idx, tick in enumerate(range(lo, hi + 1, max(1, (hi - lo) // 4 or 1))):
            y = y_of(tick, lo, hi, top, h)
            out.append(f'<line x1="{left}" y1="{y:.1f}" x2="{left + panel_w}" y2="{y:.1f}" stroke="#eef2f7"/>')
            out.append(f'<text x="{left - 8}" y="{y + 4:.1f}" text-anchor="end" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{tick}</text>')
        for i, row in enumerate(rows):
            x = x_of(i)
            if i % max(1, len(rows) // 12) == 0 or i == len(rows) - 1:
                out.append(f'<text x="{x:.1f}" y="{top + h + 18}" text-anchor="middle" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{row["step"]}</text>')
        for key, color, label in series:
            points = " ".join(
                f"{x_of(i):.1f},{y_of(row[key], lo, hi, top, h):.1f}" for i, row in enumerate(rows)
            )
            out.append(f'<polyline fill="none" stroke="{color}" stroke-width="2.4" points="{points}"/>')
            y = y_of(rows[-1][key], lo, hi, top, h)
            out.append(f'<text x="{left + panel_w + 20}" y="{y + 4:.1f}" font-size="12" font-family="Helvetica, Arial, sans-serif" fill="{color}">{label}</text>')
        return out

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="#f7f7f2"/>',
        '<text x="60" y="42" font-size="24" font-family="Helvetica, Arial, sans-serif" fill="#111827">2P2Z-Only Model</text>',
        '<text x="60" y="68" font-size="13" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">Pure compensator simulation without MPLAB peripheral/state-machine overhead</text>',
    ]
    svg += panel([("an0", "#2563eb", "AN0"), ("an2", "#f97316", "AN2")], top1, panel_h1, "ADC Inputs")
    svg += panel(
        [
            ("vout_sum", "#059669", "vout_sum"),
            ("vout_setpoint", "#dc2626", "voutSetpoint"),
            ("comp_y_out", "#0f766e", "compY_out"),
            ("llc_period_cmd", "#7c3aed", "llcPeriodCmd"),
            ("ptper_cmd", "#a16207", "ptperCommand"),
        ],
        top2,
        panel_h2,
        "2P2Z Response",
    )
    svg.append("</svg>")
    path.write_text("\n".join(svg), encoding="utf-8")


def main():
    steps = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("out/sim-2p2z")
    out_dir.mkdir(parents=True, exist_ok=True)
    rows = run_model(steps)
    csv_path = out_dir / "sim_2p2z_trace.csv"
    svg_path = out_dir / "sim_2p2z_trace.svg"
    write_csv(rows, csv_path)
    write_svg(rows, svg_path)
    tail = rows[-1]
    print(
        f"2P2Z model complete: steps={steps} final_vout_sum={tail['vout_sum']} "
        f"final_llcPeriodCmd={tail['llc_period_cmd']} final_ptper={tail['ptper_cmd']}"
    )
    print(f"Trace: csv={csv_path} svg={svg_path}")


if __name__ == "__main__":
    main()
