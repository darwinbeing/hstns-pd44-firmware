#!/usr/bin/env python3
import csv
import math
import sys


def load_rows(path):
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def scale_x(index, count, left, width):
    if count <= 1:
        return left + width / 2.0
    return left + (index * width) / float(count - 1)


def scale_y(value, lo, hi, top, height):
    if hi <= lo:
        return top + height / 2.0
    return top + height - ((value - lo) * height / float(hi - lo))


def polyline(rows, key, left, top, width, height, lo, hi):
    points = []
    for i, row in enumerate(rows):
        x = scale_x(i, len(rows), left, width)
        y = scale_y(float(row[key]), lo, hi, top, height)
        points.append(f"{x:.1f},{y:.1f}")
    return " ".join(points)


def ticks(lo, hi, count=5):
    if hi <= lo:
        return [lo]
    step = (hi - lo) / float(count - 1)
    return [lo + step * i for i in range(count)]


def panel(svg, rows, rect, title, series):
    left, top, width, height = rect
    values = []
    for key, _, _ in series:
        for row in rows:
            values.append(float(row[key]))

    lo = min(values)
    hi = max(values)
    if lo == hi:
        lo -= 1.0
        hi += 1.0
    pad = max(1.0, (hi - lo) * 0.08)
    lo -= pad
    hi += pad

    svg.append(f'<rect x="{left}" y="{top}" width="{width}" height="{height}" fill="#ffffff" stroke="#d0d7de"/>')
    svg.append(f'<text x="{left}" y="{top - 12}" font-size="16" font-family="Helvetica, Arial, sans-serif" fill="#111827">{title}</text>')

    for tick in ticks(lo, hi):
        y = scale_y(tick, lo, hi, top, height)
        svg.append(f'<line x1="{left}" y1="{y:.1f}" x2="{left + width}" y2="{y:.1f}" stroke="#eef2f7"/>')
        svg.append(f'<text x="{left - 8}" y="{y + 4:.1f}" text-anchor="end" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{int(round(tick))}</text>')

    label_stride = max(1, len(rows) // 12)
    for i, row in enumerate(rows):
        if (i % label_stride) != 0 and i != (len(rows) - 1):
            continue
        x = scale_x(i, len(rows), left, width)
        svg.append(f'<line x1="{x:.1f}" y1="{top}" x2="{x:.1f}" y2="{top + height}" stroke="#f8fafc"/>')
        svg.append(f'<text x="{x:.1f}" y="{top + height + 18}" text-anchor="middle" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{row["step"]}</text>')

    for key, color, label in series:
        points = polyline(rows, key, left, top, width, height, lo, hi)
        svg.append(f'<polyline fill="none" stroke="{color}" stroke-width="2.5" points="{points}"/>')
        last = rows[-1]
        lx = left + width + 16
        ly = scale_y(float(last[key]), lo, hi, top, height)
        svg.append(f'<line x1="{left + width + 2}" y1="{ly:.1f}" x2="{lx - 6}" y2="{ly:.1f}" stroke="{color}" stroke-width="2"/>')
        svg.append(f'<text x="{lx}" y="{ly + 4:.1f}" font-size="12" font-family="Helvetica, Arial, sans-serif" fill="{color}">{label}</text>')


def main():
    if len(sys.argv) != 3:
        print("usage: plot_sim_trace.py input.csv output.svg", file=sys.stderr)
        return 2

    rows = load_rows(sys.argv[1])
    if not rows:
        print("empty csv", file=sys.stderr)
        return 2

    width = 1200
    height = 1880
    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="#f7f7f2"/>',
        '<text x="60" y="42" font-size="24" font-family="Helvetica, Arial, sans-serif" fill="#111827">HSTNS-PD44 Simulation Trace</text>',
        '<text x="60" y="68" font-size="13" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">Main-chain simulation for T1, T2, T4 and mainStateDispatch</text>',
    ]

    panel(
        svg,
        rows,
        (70, 110, 900, 210),
        "ADC Inputs",
        [
            ("an0", "#2563eb", "AN0"),
            ("an2", "#f97316", "AN2"),
        ],
    )
    panel(
        svg,
        rows,
        (70, 380, 900, 210),
        "Setpoint Tracking",
        [
            ("vout_setpoint", "#dc2626", "voutSetpoint"),
            ("vout_setpoint_target", "#111827", "setpointTarget"),
            ("vout_target_code", "#7c3aed", "voutTargetCode"),
        ],
    )
    panel(
        svg,
        rows,
        (70, 650, 900, 210),
        "Loop Command (llcPeriodCmd)",
        [
            ("llc_period_cmd", "#7c3aed", "llcPeriodCmd"),
        ],
    )

    llc_values = [int(float(r["llc_period_cmd"])) for r in rows]
    ptper_values = [int(float(r["ptper"])) for r in rows]
    svg.append(
        f'<text x="980" y="700" font-size="12" font-family="Helvetica, Arial, sans-serif" fill="#111827">'
        f'llcPeriodCmd: start={llc_values[0]} end={llc_values[-1]} min={min(llc_values)} max={max(llc_values)}</text>'
    )
    svg.append(
        f'<text x="980" y="720" font-size="12" font-family="Helvetica, Arial, sans-serif" fill="#111827">'
        f'PTPER: start={ptper_values[0]} end={ptper_values[-1]} min={min(ptper_values)} max={max(ptper_values)}</text>'
    )

    panel(
        svg,
        rows,
        (70, 920, 900, 210),
        "PTPER / Vout / Compensator",
        [
            ("ptper", "#a16207", "PTPER"),
            ("vout_sum", "#059669", "vout_sum"),
            ("comp_y_out", "#0f766e", "compY_out"),
        ],
    )
    panel(
        svg,
        rows,
        (70, 1190, 900, 210),
        "State And Startup Gates",
        [
            ("state", "#1d4ed8", "systemState"),
            ("droop_mode", "#9333ea", "droopMode"),
            ("aux_flags", "#7c3aed", "auxFlags"),
            ("pwm_running", "#059669", "pwmRunning"),
            ("portd", "#dc2626", "PORTD"),
            ("latd", "#b91c1c", "LATD"),
            ("latf", "#be123c", "LATF"),
        ],
    )

    panel(
        svg,
        rows,
        (70, 1460, 900, 210),
        "CMP/DAC And Current Sense",
        [
            ("adcbuf4", "#2563eb", "ADCBUF4"),
            ("cmpdac3_code", "#0f766e", "CMPDAC3[9:0]"),
            ("cmp3_out", "#dc2626", "CMP3_OUT(bit3)"),
        ],
    )

    svg.append("</svg>")

    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write("\n".join(svg))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
