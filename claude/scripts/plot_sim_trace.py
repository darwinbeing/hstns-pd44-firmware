#!/usr/bin/env python3
import csv
import math
import sys

NUMERIC_FIELDS = [
    "step",
    "an0",
    "an2",
    "vout_sum",
    "vout_setpoint",
    "vout_setpoint_target",
    "llc_period_cmd",
    "comp_y_out",
    "state",
    "ptper",
    "margin_threshold",
    "aux_flags",
    "pwm_running",
    "status_flags",
    "startup_flags",
    "system_flags",
    "portd",
    "latd",
    "latf",
    "vout_target_code",
    "vref_mode_select",
    "droop_mode",
    "adcbuf4",
    "cmpcon3",
    "cmpdac3",
    "cmpcon4",
    "cmpdac4",
    "cmp3_out",
    "cmp4_out",
    "cmpdac3_code",
    "cmpdac4_code",
]


def load_rows(path):
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def parse_float(value):
    if value is None:
        return None
    value = str(value).strip()
    if not value:
        return None
    try:
        return float(value)
    except ValueError:
        return None


def sanitize_rows(rows):
    last = {}
    sanitized = []
    for i, row in enumerate(rows):
        out = dict(row)
        for key in NUMERIC_FIELDS:
            value = parse_float(out.get(key))
            if value is None:
                if key in last:
                    value = last[key]
                elif key == "step":
                    value = float(i)
                else:
                    value = 0.0
            last[key] = value
            out[key] = value
        sanitized.append(out)
    return sanitized


def scale_x(step_value, step_lo, step_hi, left, width):
    if step_hi <= step_lo:
        return left + width / 2.0
    return left + ((step_value - step_lo) * width) / float(step_hi - step_lo)


def scale_y(value, lo, hi, top, height):
    if hi <= lo:
        return top + height / 2.0
    return top + height - ((value - lo) * height / float(hi - lo))


def polyline(rows, key, left, top, width, height, lo, hi, step_lo, step_hi):
    points = []
    for row in rows:
        x = scale_x(row["step"], step_lo, step_hi, left, width)
        y = scale_y(row[key], lo, hi, top, height)
        points.append(f"{x:.1f},{y:.1f}")
    return " ".join(points)


def ticks(lo, hi, count=5):
    if hi <= lo:
        return [lo]
    step = (hi - lo) / float(count - 1)
    return [lo + step * i for i in range(count)]


def panel(svg, rows, rect, title, series):
    left, top, width, height = rect
    step_lo = min(row["step"] for row in rows)
    step_hi = max(row["step"] for row in rows)
    values = []
    for key, _, _ in series:
        for row in rows:
            values.append(row[key])

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

    for step_tick in ticks(step_lo, step_hi, 7):
        x = scale_x(step_tick, step_lo, step_hi, left, width)
        svg.append(f'<line x1="{x:.1f}" y1="{top}" x2="{x:.1f}" y2="{top + height}" stroke="#f8fafc"/>')
        svg.append(f'<text x="{x:.1f}" y="{top + height + 18}" text-anchor="middle" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{int(round(step_tick))}</text>')

    for key, color, label in series:
        points = polyline(rows, key, left, top, width, height, lo, hi, step_lo, step_hi)
        svg.append(f'<polyline fill="none" stroke="{color}" stroke-width="2.5" points="{points}"/>')

    legend_x = left + width - 170
    legend_y = top + 10
    legend_w = 160
    legend_h = 10 + (len(series) * 18)
    svg.append(
        f'<rect x="{legend_x}" y="{legend_y}" width="{legend_w}" height="{legend_h}" '
        f'fill="#ffffff" opacity="0.88" stroke="#d0d7de"/>'
    )
    for idx, (key, color, label) in enumerate(series):
        y = legend_y + (idx * 18) + 14
        svg.append(f'<line x1="{legend_x + 8}" y1="{y}" x2="{legend_x + 24}" y2="{y}" stroke="{color}" stroke-width="2.5"/>')
        svg.append(
            f'<text x="{legend_x + 30}" y="{y + 4}" font-size="11" '
            f'font-family="Helvetica, Arial, sans-serif" fill="#111827">{label}</text>'
        )


def main():
    if len(sys.argv) != 3:
        print("usage: plot_sim_trace.py input.csv output.svg", file=sys.stderr)
        return 2

    rows = sanitize_rows(load_rows(sys.argv[1]))
    if not rows:
        print("empty csv", file=sys.stderr)
        return 2

    width = 1200
    height = 1040
    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="#f7f7f2"/>',
        '<text x="60" y="42" font-size="24" font-family="Helvetica, Arial, sans-serif" fill="#111827">HSTNS-PD44 Simulation Trace</text>',
        '<text x="60" y="68" font-size="13" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">Signals: AN0, AN2, llcPeriodCmd, voutSetpoint (separate panels)</text>',
    ]

    steps = [int(r["step"]) for r in rows]
    an0_values = [int(r["an0"]) for r in rows]
    an2_values = [int(r["an2"]) for r in rows]
    llc_values = [int(r["llc_period_cmd"]) for r in rows]
    vout_sp_values = [int(r["vout_setpoint"]) for r in rows]

    svg.append(
        f'<text x="60" y="94" font-size="12" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">'
        f'step {steps[0]} -> {steps[-1]} | AN0 {an0_values[0]}->{an0_values[-1]} | AN2 {an2_values[0]}->{an2_values[-1]} | '
        f'llc {llc_values[0]}->{llc_values[-1]} | voutSetpoint {vout_sp_values[0]}->{vout_sp_values[-1]}</text>'
    )

    panel(
        svg,
        rows,
        (70, 120, 1040, 220),
        "ADC Feedback (AN0 / AN2)",
        [
            ("an0", "#2563eb", "AN0"),
            ("an2", "#f97316", "AN2"),
        ],
    )

    panel(
        svg,
        rows,
        (70, 420, 1040, 220),
        "llcPeriodCmd",
        [
            ("llc_period_cmd", "#7c3aed", "llcPeriodCmd"),
        ],
    )

    panel(
        svg,
        rows,
        (70, 720, 1040, 220),
        "voutSetpoint",
        [
            ("vout_setpoint", "#dc2626", "voutSetpoint"),
        ],
    )

    svg.append("</svg>")

    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write("\n".join(svg))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
