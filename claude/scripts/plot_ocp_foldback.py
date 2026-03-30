#!/usr/bin/env python3
import csv
import sys
from pathlib import Path


def _ticks(vmin: float, vmax: float, n: int = 6):
    if vmax <= vmin:
        return [vmin]
    step = (vmax - vmin) / float(n - 1)
    return [vmin + i * step for i in range(n)]


def _polyline(rows, x_key, y_key, x0, x1, y0, y1):
    xs = [float(r[x_key]) for r in rows]
    ys = [float(r[y_key]) for r in rows]
    xmin, xmax = min(xs), max(xs)
    ymin, ymax = min(ys), max(ys)
    if xmax <= xmin:
        xmax = xmin + 1.0
    if ymax <= ymin:
        ymax = ymin + 1.0
    points = []
    for r in rows:
        x = float(r[x_key])
        y = float(r[y_key])
        px = x0 + (x - xmin) * (x1 - x0) / (xmax - xmin)
        py = y1 - (y - ymin) * (y1 - y0) / (ymax - ymin)
        points.append(f"{px:.2f},{py:.2f}")
    return " ".join(points), xmin, xmax, ymin, ymax


def _panel(svg, rows, rect, title, x_key, y_key, y_color, y_label):
    left, top, width, height = rect
    x0, x1 = left + 56, left + width - 18
    y0, y1 = top + 16, top + height - 34

    svg.append(f'<rect x="{left}" y="{top}" width="{width}" height="{height}" fill="#ffffff" stroke="#d0d7de"/>')
    svg.append(f'<text x="{left + 8}" y="{top + 14}" font-size="13" font-family="Helvetica, Arial, sans-serif" fill="#111827">{title}</text>')

    points, xmin, xmax, ymin, ymax = _polyline(rows, x_key, y_key, x0, x1, y0, y1)

    for t in _ticks(ymin, ymax):
        py = y1 - (t - ymin) * (y1 - y0) / (ymax - ymin if ymax > ymin else 1.0)
        svg.append(f'<line x1="{x0}" y1="{py:.2f}" x2="{x1}" y2="{py:.2f}" stroke="#eef2f7"/>')
        svg.append(f'<text x="{x0 - 6}" y="{py + 4:.2f}" text-anchor="end" font-size="10" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{int(round(t))}</text>')

    for t in _ticks(xmin, xmax):
        px = x0 + (t - xmin) * (x1 - x0) / (xmax - xmin if xmax > xmin else 1.0)
        svg.append(f'<line x1="{px:.2f}" y1="{y0}" x2="{px:.2f}" y2="{y1}" stroke="#f5f7fa"/>')
        svg.append(f'<text x="{px:.2f}" y="{y1 + 16}" text-anchor="middle" font-size="10" font-family="Helvetica, Arial, sans-serif" fill="#4b5563">{int(round(t))}</text>')

    svg.append(f'<polyline fill="none" stroke="{y_color}" stroke-width="2.2" points="{points}"/>')
    svg.append(f'<text x="{(x0 + x1) / 2:.2f}" y="{top + height - 8}" text-anchor="middle" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="#374151">{x_key}</text>')
    svg.append(f'<text x="{x0 + 4}" y="{y0 + 12}" font-size="11" font-family="Helvetica, Arial, sans-serif" fill="{y_color}">{y_label}</text>')


def main():
    if len(sys.argv) != 3:
        print("usage: plot_ocp_foldback.py input.csv output.svg", file=sys.stderr)
        return 2

    in_path = Path(sys.argv[1])
    out_path = Path(sys.argv[2])
    with in_path.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
    if not rows:
        print("empty input csv", file=sys.stderr)
        return 2

    width, height = 1220, 760
    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect x="0" y="0" width="100%" height="100%" fill="#f8fafc"/>',
        '<text x="18" y="28" font-size="18" font-family="Helvetica, Arial, sans-serif" fill="#0f172a">ocpFoldback sweep</text>',
    ]

    _panel(
        svg,
        rows,
        (14, 52, 1192, 330),
        "vrefOcpAdj vs vout_sum",
        "vout_sum",
        "vrefOcpAdj",
        "#2563eb",
        "vrefOcpAdj",
    )
    _panel(
        svg,
        rows,
        (14, 400, 1192, 330),
        "gain_scale (ocpError) vs vout_sum",
        "vout_sum",
        "gain_scale",
        "#f97316",
        "gain_scale",
    )

    svg.append("</svg>")
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text("\n".join(svg), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
