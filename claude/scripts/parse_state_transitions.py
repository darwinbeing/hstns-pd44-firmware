#!/usr/bin/env python3
import argparse
import csv
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description="Extract systemState transitions from SCL CSV trace.")
    parser.add_argument("csv_file", help="Path to scl_trace.csv")
    args = parser.parse_args()

    csv_path = Path(args.csv_file)
    if not csv_path.exists():
        print(f"error: csv not found: {csv_path}")
        return 2

    rows = list(csv.DictReader(csv_path.open(encoding="utf-8", newline="")))
    if not rows:
        print("error: csv has no rows")
        return 2

    series = []
    for i, row in enumerate(rows):
        raw = row.get("systemState", "")
        if raw == "":
            continue
        series.append((i, int(raw, 0)))

    if not series:
        print("error: systemState column is empty")
        return 2

    transitions = []
    first_sample, first_state = series[0]
    prev_state = first_state
    for sample, state in series[1:]:
        if state != prev_state:
            transitions.append((sample, prev_state, state))
            prev_state = state

    mask = 0
    for _, state in series:
        if 0 <= state < 16:
            mask |= (1 << state)

    print(
        "state_trace: "
        f"samples={len(series)} first={first_state} last={series[-1][1]} "
        f"state_mask=0x{mask:X} transition_count={len(transitions)}"
    )
    if transitions:
        for sample, from_state, to_state in transitions:
            print(f"transition sample={sample} {from_state}->{to_state}")
    else:
        print("transition none")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
