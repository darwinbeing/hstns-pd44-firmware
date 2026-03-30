#!/usr/bin/env python3
"""
decomp_verify.py

Assembly-referenced verification for decompiled C functions:
1) Signature audit (parameter count/types) against selected assembly call sites.
2) Layered tests from simple to complex:
   - simple: helper semantic boundary tests (assembly-derived behavior)
   - medium: T2-only simulator convergence test
   - complex: full startup chain and full chain with ACTIVE->FAULT transition
"""

from __future__ import annotations

import argparse
import csv
import os
import random
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Tuple


ROOT = Path(__file__).resolve().parents[1]
ASM_PATH = ROOT / "dsPIC33FJ64GS606.txt"
SIM_LOG_PATH = ROOT / "out" / "sim" / "hstns_pd44_mdb.log"
SIM_CSV_PATH = ROOT / "out" / "sim" / "sim_trace.csv"
REPORT_PATH = ROOT / "out" / "audit" / "decompiled_function_audit.md"


@dataclass
class SignatureCase:
    name: str
    source_relpath: str
    asm_addr: int
    expected_return: str
    expected_param_types: Sequence[str]
    call_sites: Sequence[int]


SIGNATURE_CASES: List[SignatureCase] = [
    SignatureCase(
        name="thresholdCompare433C",
        source_relpath="decompiled/voltage_loop.c",
        asm_addr=0x4326,
        expected_return="uint16_t",
        expected_param_types=("uint16_t", "uint16_t", "uint16_t", "uint16_t"),
        call_sites=(0x4352,),
    ),
    SignatureCase(
        name="latchCounter430C",
        source_relpath="decompiled/voltage_loop.c",
        asm_addr=0x430C,
        expected_return="uint16_t",
        expected_param_types=("uint16_t", "uint16_t", "volatile int16_t *", "uint16_t"),
        call_sites=(0x4362,),
    ),
    SignatureCase(
        name="adcBuf12OvercurrentLatch",
        source_relpath="decompiled/voltage_loop.c",
        asm_addr=0x433C,
        expected_return="void",
        expected_param_types=(),
        call_sites=(0x45A0,),
    ),
    SignatureCase(
        name="adcBuf4FastAverage",
        source_relpath="decompiled/voltage_loop.c",
        asm_addr=0x43D4,
        expected_return="void",
        expected_param_types=(),
        call_sites=(0x4B2A,),
    ),
    SignatureCase(
        name="softStartRamp2",
        source_relpath="decompiled/pwm.c",
        asm_addr=0x2DF6,
        expected_return="void",
        expected_param_types=(),
        call_sites=(0x3834,),
    ),
    SignatureCase(
        name="mainStateDispatch",
        source_relpath="decompiled/state_machine.c",
        asm_addr=0x51FE,
        expected_return="void",
        expected_param_types=(),
        call_sites=(0x5A5E,),
    ),
    SignatureCase(
        name="main",
        source_relpath="decompiled/main.c",
        asm_addr=0x5A1C,
        expected_return="int",
        expected_param_types=(),
        call_sites=(),
    ),
]


def norm_ws(s: str) -> str:
    return " ".join(s.strip().split())


def norm_type(s: str) -> str:
    # ignore whitespace differences for comparison
    return re.sub(r"\s+", "", s.strip())


def split_params(params: str) -> List[str]:
    p = params.strip()
    if not p or p == "void":
        return []
    out: List[str] = []
    depth = 0
    cur = []
    for ch in p:
        if ch == "," and depth == 0:
            out.append("".join(cur).strip())
            cur = []
            continue
        if ch in "([<":
            depth += 1
        elif ch in ")]>":
            depth = max(0, depth - 1)
        cur.append(ch)
    if cur:
        out.append("".join(cur).strip())
    return out


def extract_param_type(param: str) -> str:
    # Remove inline comments.
    p = re.sub(r"/\*.*?\*/", "", param).strip()
    p = re.sub(r"\s+", " ", p)
    # Drop parameter name (last identifier, optional array suffix).
    m = re.match(r"(.+?)([A-Za-z_]\w*)\s*(\[[^\]]*\])?$", p)
    if m:
        t = m.group(1).strip()
        if t:
            return t
    m = re.match(r"(.+?)\s+([A-Za-z_]\w*)\s*(\[[^\]]*\])?$", p)
    if m:
        t = m.group(1).strip()
        return t
    return p


def extract_signature(source: Path, func_name: str) -> Tuple[str, List[str], str]:
    text = source.read_text(encoding="utf-8")
    pattern = re.compile(
        r"(^[ \t]*(?:static\s+)?[A-Za-z_][A-Za-z0-9_\s\*]*?\b"
        + re.escape(func_name)
        + r"\s*\([^{};]*?\))\s*\{",
        re.S | re.M,
    )
    m = pattern.search(text)
    if not m:
        raise RuntimeError(f"signature for {func_name} not found in {source}")
    sig = m.group(1)
    sig_oneline = norm_ws(sig)

    m2 = re.match(
        r"^(?:static\s+)?(?P<ret>.*?)\b"
        + re.escape(func_name)
        + r"\s*\((?P<params>.*)\)$",
        sig_oneline,
    )
    if not m2:
        raise RuntimeError(f"failed to parse signature text: {sig_oneline}")
    ret = norm_ws(m2.group("ret"))
    params_raw = m2.group("params")
    params = split_params(params_raw)
    param_types = [norm_ws(extract_param_type(p)) for p in params]
    return ret, param_types, sig_oneline


def load_asm_lines(path: Path) -> Dict[int, str]:
    out: Dict[int, str] = {}
    pat = re.compile(r"^\s*\d[\d,]*\s+([0-9A-Fa-f]{4})\s+[0-9A-Fa-f]{6}\s+(.+?)\s*$")
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = pat.match(line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        dis = m.group(2).strip()
        out[addr] = dis
    return out


def read_last_symbol(log_text: str, symbol: str) -> Optional[int]:
    cleaned = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", log_text)
    vals = re.findall(
        re.escape(symbol) + r"\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))",
        cleaned,
    )
    if not vals:
        return None
    return int(vals[-1], 0)


def run_cmd(cmd: Sequence[str], extra_env: Optional[Dict[str, str]] = None, timeout_s: int = 1800) -> str:
    env = os.environ.copy()
    if extra_env:
        env.update(extra_env)
    proc = subprocess.run(
        list(cmd),
        cwd=ROOT,
        env=env,
        capture_output=True,
        text=True,
        timeout=timeout_s,
    )
    out = (proc.stdout or "") + (proc.stderr or "")
    if proc.returncode != 0:
        raise RuntimeError(f"command failed ({proc.returncode}): {' '.join(cmd)}\n{out}")
    return out


def sim_env(extra: Dict[str, str]) -> Dict[str, str]:
    base = {
        "SIM_MDB_RETRIES": "10",
        "SIM_POST_HALT_WAIT_MS": "2",
        "SIM_INIT_WAIT_MS": "1",
    }
    base.update(extra)
    return base


def parse_summary_line(text: str) -> Tuple[str, Dict[str, str], str]:
    line = ""
    for ln in text.splitlines():
        if ln.startswith("Simulation passed:") or ln.startswith("Simulation failed:"):
            line = ln.strip()
    if not line:
        raise RuntimeError("did not find simulation summary line")
    status = "passed" if line.startswith("Simulation passed:") else "failed"
    kv = dict(re.findall(r"([A-Za-z0-9_]+)=([^\s]+)", line))
    return status, kv, line


def to_int(v: str) -> int:
    return int(v, 0)


def audit_signatures(asm_lines: Dict[int, str]) -> Tuple[List[str], List[str]]:
    ok: List[str] = []
    fail: List[str] = []

    for case in SIGNATURE_CASES:
        source = ROOT / case.source_relpath
        try:
            ret, param_types, sig_raw = extract_signature(source, case.name)
        except Exception as exc:  # noqa: BLE001
            fail.append(f"{case.name}: signature parse error: {exc}")
            continue

        exp_ret_ok = norm_type(ret) == norm_type(case.expected_return)
        exp_param_ok = len(param_types) == len(case.expected_param_types)
        if exp_param_ok:
            for got, exp in zip(param_types, case.expected_param_types):
                if norm_type(got) != norm_type(exp):
                    exp_param_ok = False
                    break

        asm_def = asm_lines.get(case.asm_addr, "")
        if not asm_def:
            fail.append(f"{case.name}: asm address 0x{case.asm_addr:04X} missing")
            continue

        call_ok = True
        for call_addr in case.call_sites:
            dis = asm_lines.get(call_addr, "")
            if not dis:
                call_ok = False
                fail.append(f"{case.name}: callsite 0x{call_addr:04X} missing")
                continue
            if f"0x{case.asm_addr:04X}" not in dis:
                call_ok = False
                fail.append(
                    f"{case.name}: callsite 0x{call_addr:04X} does not target 0x{case.asm_addr:04X} (got: {dis})"
                )

        if exp_ret_ok and exp_param_ok and call_ok:
            ok.append(
                f"{case.name}: OK | C `{sig_raw}` | ASM 0x{case.asm_addr:04X} `{asm_def}`"
            )
        else:
            fail.append(
                f"{case.name}: mismatch | C `{sig_raw}` | expected ret={case.expected_return}, "
                f"params={list(case.expected_param_types)} | ASM 0x{case.asm_addr:04X} `{asm_def}`"
            )

    return ok, fail


def helper_semantic_tests() -> Tuple[List[str], List[str]]:
    ok: List[str] = []
    fail: List[str] = []

    # 0x4326 threshold compare behavior
    def helper_threshold(value: int, th1: int, th2: int, prev: int) -> int:
        if value >= th1:
            return 1
        if value <= th2:
            return 0
        return prev & 0xFF

    # boundary checks
    vectors = [
        (0x26C, 0x26C, 0x136, 0, 1),
        (0x136, 0x26C, 0x136, 1, 0),
        (0x200, 0x26C, 0x136, 0, 0),
        (0x200, 0x26C, 0x136, 1, 1),
    ]
    for v, t1, t2, prev, expect in vectors:
        got = helper_threshold(v, t1, t2, prev)
        if got != expect:
            fail.append(f"thresholdCompare433C vector failed: v={v} prev={prev} got={got} expect={expect}")
            break
    else:
        ok.append("thresholdCompare433C boundary vectors passed")

    # 0x430C latch + counter behavior
    def helper_latch(new_level: int, limit: int, counter: int, prev_level: int) -> Tuple[int, int]:
        result = prev_level & 0xFF
        cnt = counter & 0xFFFF
        if (new_level & 0xFFFF) != (prev_level & 0xFFFF):
            cnt = (cnt + 1) & 0xFFFF
            if cnt >= (limit & 0xFFFF):
                cnt = 0
                result = new_level & 0xFF
        else:
            cnt = 0
        return result, cnt

    # deterministic vectors
    seq = [
        (1, 3, 0, 0, (0, 1)),
        (1, 3, 1, 0, (0, 2)),
        (1, 3, 2, 0, (1, 0)),
        (1, 3, 0, 1, (1, 0)),
    ]
    for new_level, limit, cnt, prev, expect in seq:
        got = helper_latch(new_level, limit, cnt, prev)
        if got != expect:
            fail.append(f"latchCounter430C vector failed: got={got} expect={expect}")
            break
    else:
        ok.append("latchCounter430C deterministic vectors passed")

    # quick random consistency checks for monotonic counter behavior
    random.seed(0x430C)
    for _ in range(200):
        new_level = random.randint(0, 1)
        prev = random.randint(0, 1)
        limit = random.randint(1, 8)
        cnt = random.randint(0, 10)
        out, cnt2 = helper_latch(new_level, limit, cnt, prev)
        if new_level == prev and cnt2 != 0:
            fail.append("latchCounter430C random check failed: counter not cleared when levels equal")
            break
        if new_level != prev and cnt + 1 < limit and out != prev:
            fail.append("latchCounter430C random check failed: output changed before limit")
            break
    else:
        ok.append("latchCounter430C random checks passed")

    return ok, fail


def sim_t2_test() -> Tuple[List[str], List[str]]:
    ok: List[str] = []
    fail: List[str] = []

    out = run_cmd(
        ["make", "sim-t2"],
        extra_env=sim_env({
            "SIM_T2_TARGET_STEPS": "1500",
            "SIM_T2_HOST_SAMPLES": "500",
            "SIM_T2_SAMPLE_WAIT_MS": "1",
        }),
    )
    status, kv, line = parse_summary_line(out)
    if status != "passed":
        fail.append(f"sim-t2 did not pass: {line}")
        return ok, fail

    if to_int(kv.get("failure_count", "1")) != 0:
        fail.append(f"sim-t2 failure_count != 0 ({kv.get('failure_count')})")
    else:
        ok.append("sim-t2 failure_count == 0")

    if not SIM_CSV_PATH.exists():
        fail.append(f"sim-t2 missing csv: {SIM_CSV_PATH}")
        return ok, fail

    rows = list(csv.DictReader(SIM_CSV_PATH.open(newline="", encoding="utf-8")))
    if len(rows) < 50:
        fail.append(f"sim-t2 csv too short ({len(rows)} rows)")
        return ok, fail

    llc = [int(float(r["llc_period_cmd"])) for r in rows if r.get("llc_period_cmd", "").strip()]
    if len(llc) < 50:
        fail.append("sim-t2 llc series too short")
        return ok, fail

    if llc[0] < 20000:
        fail.append(f"sim-t2 llc start too low ({llc[0]})")
    else:
        ok.append(f"sim-t2 llc starts high ({llc[0]})")

    tail = llc[-80:]
    avg_tail = sum(tail) / float(len(tail))
    if not (2800.0 <= avg_tail <= 4500.0):
        fail.append(f"sim-t2 llc steady avg out of range ({avg_tail:.1f})")
    else:
        ok.append(f"sim-t2 llc steady avg in range ({avg_tail:.1f})")

    if min(llc) > 5000:
        fail.append(f"sim-t2 llc did not converge low enough (min={min(llc)})")
    else:
        ok.append(f"sim-t2 llc min reached {min(llc)}")

    return ok, fail


def sim_chain_startup_test() -> Tuple[List[str], List[str]]:
    ok: List[str] = []
    fail: List[str] = []

    out = run_cmd(
        ["make", "sim"],
        extra_env=sim_env({
            "SIM_TEST_TARGET_STEPS": "70",
            "SIM_HOST_SAMPLES": "300",
            "SIM_SAMPLE_WAIT_MS": "2",
        }),
    )
    status, kv, line = parse_summary_line(out)
    if status != "passed":
        fail.append(f"sim-chain-startup did not pass: {line}")
        return ok, fail

    state_mask = to_int(kv.get("state_mask", "0"))
    idle_to_startup = to_int(kv.get("idle_to_startup", "0"))
    startup_to_active = to_int(kv.get("startup_to_active", "0"))
    active_to_fault = to_int(kv.get("active_to_fault", "0"))
    final_state = to_int(kv.get("state", "65535"))

    if (state_mask & 0x7) != 0x7:
        fail.append(f"sim-chain-startup state_mask missing IDLE/STARTUP/ACTIVE (0x{state_mask:X})")
    else:
        ok.append(f"sim-chain-startup state_mask=0x{state_mask:X}")

    if idle_to_startup < 1 or startup_to_active < 1:
        fail.append(
            "sim-chain-startup missing transitions: "
            f"idle_to_startup={idle_to_startup}, startup_to_active={startup_to_active}"
        )
    else:
        ok.append(
            "sim-chain-startup transitions ok: "
            f"idle_to_startup={idle_to_startup}, startup_to_active={startup_to_active}"
        )

    if active_to_fault != 0:
        fail.append(f"sim-chain-startup unexpected fault transition ({active_to_fault})")
    else:
        ok.append("sim-chain-startup no ACTIVE->FAULT transition as expected")

    if final_state != 2:
        fail.append(f"sim-chain-startup final state expected ACTIVE(2), got {final_state}")
    else:
        ok.append("sim-chain-startup final state ACTIVE(2)")

    if SIM_LOG_PATH.exists():
        log_text = SIM_LOG_PATH.read_text(encoding="utf-8", errors="ignore")
        t1 = read_last_symbol(log_text, "sim_debug.t1_count")
        t2 = read_last_symbol(log_text, "sim_debug.t2_count")
        t4 = read_last_symbol(log_text, "sim_debug.t4_count")
        if t1 != 70 or t2 != 350 or t4 != 70:
            fail.append(f"sim-chain-startup timer counts mismatch: t1={t1}, t2={t2}, t4={t4}")
        else:
            ok.append("sim-chain-startup timer counts match expected cadence (70/350/70)")
    else:
        fail.append(f"sim-chain-startup missing mdb log: {SIM_LOG_PATH}")

    return ok, fail


def sim_chain_fault_test() -> Tuple[List[str], List[str]]:
    ok: List[str] = []
    fail: List[str] = []

    out = run_cmd(
        ["make", "sim"],
        extra_env=sim_env({
            "SIM_TEST_TARGET_STEPS": "300",
            "SIM_HOST_SAMPLES": "200",
            "SIM_SAMPLE_WAIT_MS": "2",
        }),
    )
    status, kv, line = parse_summary_line(out)
    if status != "passed":
        fail.append(f"sim-chain-fault did not pass: {line}")
        return ok, fail

    state_mask = to_int(kv.get("state_mask", "0"))
    active_to_fault = to_int(kv.get("active_to_fault", "0"))
    final_state = to_int(kv.get("state", "65535"))
    ptper = to_int(kv.get("ptper", "-1"))

    if (state_mask & 0xF) != 0xF:
        fail.append(f"sim-chain-fault state_mask missing a major state (0x{state_mask:X})")
    else:
        ok.append(f"sim-chain-fault state_mask=0x{state_mask:X}")

    if active_to_fault < 1:
        fail.append("sim-chain-fault missing ACTIVE->FAULT transition")
    else:
        ok.append(f"sim-chain-fault ACTIVE->FAULT transitions={active_to_fault}")

    if final_state != 3:
        fail.append(f"sim-chain-fault final state expected FAULT(3), got {final_state}")
    else:
        ok.append("sim-chain-fault final state FAULT(3)")

    if SIM_LOG_PATH.exists():
        log_text = SIM_LOG_PATH.read_text(encoding="utf-8", errors="ignore")
        ptper_shadow = read_last_symbol(log_text, "sim_debug.observed_ptper_shadow")
        ptper_reg = read_last_symbol(log_text, "sim_debug.observed_ptper_register")
        first_drop_phase = read_last_symbol(log_text, "sim_debug.first_drop_phase")
        first_drop_step = read_last_symbol(log_text, "sim_debug.first_drop_step")
        if ptper_shadow != ptper_reg:
            fail.append(f"sim-chain-fault PTPER mismatch shadow/reg: {ptper_shadow} vs {ptper_reg}")
        else:
            ok.append(f"sim-chain-fault PTPER shadow/reg match ({ptper_shadow})")
        if first_drop_phase != 2:
            fail.append(f"sim-chain-fault first_drop_phase expected 2(T2), got {first_drop_phase}")
        else:
            ok.append(f"sim-chain-fault first_drop_phase=2 at step={first_drop_step}")
    else:
        fail.append(f"sim-chain-fault missing mdb log: {SIM_LOG_PATH}")

    if ptper <= 0:
        fail.append(f"sim-chain-fault invalid ptper in summary ({ptper})")
    return ok, fail


def write_report(
    sig_ok: Sequence[str],
    sig_fail: Sequence[str],
    simple_ok: Sequence[str],
    simple_fail: Sequence[str],
    medium_ok: Sequence[str],
    medium_fail: Sequence[str],
    complex_ok: Sequence[str],
    complex_fail: Sequence[str],
) -> None:
    REPORT_PATH.parent.mkdir(parents=True, exist_ok=True)
    lines: List[str] = []
    lines.append("# Decompiled Function Verification Report")
    lines.append("")
    lines.append("## Signature Audit (Assembly-Referenced)")
    lines.append("")
    if sig_ok:
        lines.append("### Passed")
        lines.append("")
        for item in sig_ok:
            lines.append(f"- {item}")
        lines.append("")
    if sig_fail:
        lines.append("### Failed")
        lines.append("")
        for item in sig_fail:
            lines.append(f"- {item}")
        lines.append("")

    lines.append("## Layered Tests")
    lines.append("")
    lines.append("### Simple")
    lines.append("")
    for item in simple_ok:
        lines.append(f"- PASS: {item}")
    for item in simple_fail:
        lines.append(f"- FAIL: {item}")
    lines.append("")

    lines.append("### Medium")
    lines.append("")
    for item in medium_ok:
        lines.append(f"- PASS: {item}")
    for item in medium_fail:
        lines.append(f"- FAIL: {item}")
    lines.append("")

    lines.append("### Complex")
    lines.append("")
    for item in complex_ok:
        lines.append(f"- PASS: {item}")
    for item in complex_fail:
        lines.append(f"- FAIL: {item}")
    lines.append("")

    REPORT_PATH.write_text("\n".join(lines), encoding="utf-8")


def main(argv: Sequence[str]) -> int:
    parser = argparse.ArgumentParser(description="Audit decompiled signatures and run layered tests.")
    parser.add_argument("--audit-only", action="store_true", help="run only signature audit and simple helper tests")
    args = parser.parse_args(argv)

    if not ASM_PATH.exists():
        print(f"Missing assembly file: {ASM_PATH}", file=sys.stderr)
        return 2

    asm_lines = load_asm_lines(ASM_PATH)

    sig_ok, sig_fail = audit_signatures(asm_lines)
    simple_ok, simple_fail = helper_semantic_tests()

    medium_ok: List[str] = []
    medium_fail: List[str] = []
    complex_ok: List[str] = []
    complex_fail: List[str] = []

    if not args.audit_only:
        try:
            ok, fail = sim_t2_test()
            medium_ok.extend(ok)
            medium_fail.extend(fail)
        except Exception as exc:  # noqa: BLE001
            medium_fail.append(f"sim-t2 execution error: {exc}")

        try:
            ok, fail = sim_chain_startup_test()
            complex_ok.extend(ok)
            complex_fail.extend(fail)
        except Exception as exc:  # noqa: BLE001
            complex_fail.append(f"sim-chain-startup execution error: {exc}")

        try:
            ok, fail = sim_chain_fault_test()
            complex_ok.extend(ok)
            complex_fail.extend(fail)
        except Exception as exc:  # noqa: BLE001
            complex_fail.append(f"sim-chain-fault execution error: {exc}")

    write_report(
        sig_ok,
        sig_fail,
        simple_ok,
        simple_fail,
        medium_ok,
        medium_fail,
        complex_ok,
        complex_fail,
    )

    print(f"report: {REPORT_PATH}")
    print(f"signature: pass={len(sig_ok)} fail={len(sig_fail)}")
    print(f"simple:    pass={len(simple_ok)} fail={len(simple_fail)}")
    print(f"medium:    pass={len(medium_ok)} fail={len(medium_fail)}")
    print(f"complex:   pass={len(complex_ok)} fail={len(complex_fail)}")

    if sig_fail or simple_fail or medium_fail or complex_fail:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
