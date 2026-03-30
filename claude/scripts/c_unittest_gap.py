#!/usr/bin/env python3
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DECOMPILED = ROOT / "decompiled"
TEST_FILE = ROOT / "tests" / "c_unittest" / "test_decompiled_unittest.c"

RET_TYPES = (
    "void",
    "uint16_t",
    "int16_t",
    "uint32_t",
    "int32_t",
    "u16",
    "s16",
    "u32",
    "s32",
    "int",
    "char",
    "uint8_t",
    "int8_t",
)
FUNC_DEF_RE = re.compile(
    r"^\s*(?:static\s+)?(?:" + "|".join(re.escape(x) for x in RET_TYPES) + r")\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(",
)
FP_RE = re.compile(r"\(\*f\d+[A-Za-z0-9_]*\)\([^)]*\)\s*=\s*([A-Za-z_][A-Za-z0-9_]*)\s*;")

EXCLUDE = {
    "__attribute__",
}


def main() -> int:
    all_funcs = []
    for p in sorted(DECOMPILED.glob("*.c")):
        lines = p.read_text(encoding="utf-8", errors="ignore").splitlines()
        i = 0
        while i < len(lines):
            line = lines[i]
            m = FUNC_DEF_RE.match(line)
            if not m:
                i += 1
                continue
            name = m.group(1)
            sig = line
            j = i
            while ("{" not in sig) and (";" not in sig) and (j + 1 < len(lines)):
                j += 1
                sig += " " + lines[j].strip()
            i = j + 1
            if ";" in sig and "{" not in sig:
                continue
            if name in EXCLUDE:
                continue
            all_funcs.append((p.name, name))

    test_txt = TEST_FILE.read_text(encoding="utf-8", errors="ignore")
    covered = set(FP_RE.findall(test_txt))

    unique = []
    seen = set()
    for item in all_funcs:
        if item in seen:
            continue
        seen.add(item)
        unique.append(item)

    missing = [(f, n) for (f, n) in unique if n not in covered]
    print(f"total_functions={len(unique)}")
    print(f"covered_by_signature_unittest={len(unique) - len(missing)}")
    print(f"missing={len(missing)}")
    for f, n in missing[:200]:
        print(f"{f}:{n}")
    return 1 if missing else 0


if __name__ == "__main__":
    raise SystemExit(main())
