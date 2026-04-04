#!/usr/bin/env python3
import argparse
import csv
from pathlib import Path


def to_int(raw: str):
    if raw is None or raw == "":
        return None
    return int(raw, 0)


def fmt(v):
    if v is None:
        return "-"
    return str(v)


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Analyze SCL trace for startup state jumps and PWM/GPIO hardware checks."
    )
    ap.add_argument("csv_file", help="Path to scl_trace.csv")
    args = ap.parse_args()

    csv_path = Path(args.csv_file)
    if not csv_path.exists():
        print(f"error: csv not found: {csv_path}")
        return 2

    rows = list(csv.DictReader(csv_path.open(encoding="utf-8", newline="")))
    if not rows:
        print("error: csv has no data rows")
        return 2

    if "systemState" not in rows[0]:
        print("error: csv missing systemState column")
        return 2

    transitions = []
    prev = to_int(rows[0].get("systemState", ""))
    for i, row in enumerate(rows[1:], 1):
        cur = to_int(row.get("systemState", ""))
        if cur is None or prev is None:
            continue
        if cur != prev:
            transitions.append((i, prev, cur))
            prev = cur

    print(f"trace: samples={len(rows)} transitions={len(transitions)}")
    if not transitions:
        print("state: no transition")
        return 0

    print("state transitions:")
    for i, s0, s1 in transitions:
        print(f"  sample {i}: {s0} -> {s1}")

    # state0 gate check (if debug columns exist)
    if "dbg_state0_fail" in rows[0]:
        first_pass = None
        for i, r in enumerate(rows):
            if to_int(r.get("systemState", "")) == 0 and to_int(r.get("dbg_state0_fail", "")) == 0:
                first_pass = i
                break
        if first_pass is None:
            for i, s0, s1 in transitions:
                if s0 == 0 and s1 == 1:
                    first_pass = i
                    break
        if first_pass is not None:
            rr = rows[first_pass]
            print(
                "state0 gate pass: "
                f"sample={first_pass} marginThreshold={fmt(to_int(rr.get('marginThreshold', '')))} "
                f"auxFlags={fmt(to_int(rr.get('auxFlags', '')))} "
                f"pwmRunning={fmt(to_int(rr.get('pwmRunning', '')))}"
            )
        else:
            print("state0 gate pass: not observed")

    # show key snapshots around each transition
    watch = [
        "PORTD",
        "LATD",
        "LATF",
        "IOCON1",
        "IOCON2",
        "IOCON3",
        "PTPER",
        "ptperCommand",
        "phase3_target",
        "phase3ClampTarget",
        "statusFlags",
        "auxFlags",
        "currentLimitFlags",
        "protStatusByte",
        "IFS3",
        "IEC3",
        "PTCON",
    ]
    watch = [w for w in watch if w in rows[0]]
    print("transition snapshots:")
    for i, s0, s1 in transitions:
        if i >= len(rows):
            continue
        b = rows[i - 1] if i > 0 else rows[i]
        a = rows[i]
        print(f"  sample {i} ({s0}->{s1}):")
        for k in watch:
            vb = to_int(b.get(k, ""))
            va = to_int(a.get(k, ""))
            if vb is None and va is None:
                continue
            if vb == va:
                print(f"    {k}: {fmt(va)}")
            else:
                print(f"    {k}: {fmt(vb)} -> {fmt(va)}")

    # heuristic: PSEM ISR likely not entering if phase3_target never changes and
    # PTPER stays fixed while ptperCommand changes.
    likely_no_psem = False
    if all(col in rows[0] for col in ("PTPER", "ptperCommand", "phase3_target")):
        ptper_vals = [to_int(r["PTPER"]) for r in rows if to_int(r.get("PTPER", "")) is not None]
        cmd_vals = [to_int(r["ptperCommand"]) for r in rows if to_int(r.get("ptperCommand", "")) is not None]
        phase_vals = [to_int(r["phase3_target"]) for r in rows if to_int(r.get("phase3_target", "")) is not None]
        if ptper_vals and cmd_vals and phase_vals:
            if min(ptper_vals) == max(ptper_vals) and min(cmd_vals) != max(cmd_vals) and min(phase_vals) == max(phase_vals):
                likely_no_psem = True

    print(
        "psem heuristic: "
        + ("likely not triggering (_PWMSpEventMatchInterrupt side effects not observed)"
           if likely_no_psem else "inconclusive")
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
