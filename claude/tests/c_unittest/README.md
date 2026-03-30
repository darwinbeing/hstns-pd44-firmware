# C Unit Tests For Decompiled Functions

Run:

```bash
make c-unittest
make c-unittest-cov
make c-unittest-gap
make c-unittest-ocp-sweep
```

`ocpFoldback` sweep output:

- CSV: `out/c_unittest/ocp_foldback_sweep.csv`
- SVG: `out/c_unittest/ocp_foldback_sweep.svg`

Adjust sweep parameters from command line, for example:

```bash
make c-unittest-ocp-sweep OCP_SWEEP_VOUT_START=0 OCP_SWEEP_VOUT_END=4000 OCP_SWEEP_VOUT_STEP=10 OCP_SWEEP_IMEAS=500 OCP_SWEEP_IREF=100
```

Binary output:

- `out/c_unittest/test_decompiled_unittest`

Covered functions and assembly mapping:

- `thresholdCompare433C`  -> `0x4326`
- `latchCounter430C`      -> `0x430C`
- `adcBuf4FastAverage`    -> `0x43D4`
- `softStartRamp2`        -> `0x2DF6`
- `adcBuf12OvercurrentLatch` -> `0x433C`
- `thresholdCompare`      -> `0x30E2`
- `debounceCounter`       -> `0x31B2`
- `clampToward`           -> `0x3242`
- `crc16Update`           -> `0x5B74`
- `crc16Block`            -> `0x5B86`
- `eepromSetConfig`       -> `0x5A88`
- `eepromSetCrcLo`        -> `0x5ACC`
- `eepromSetPageAddr`     -> `0x5AD4`
- `eepromGetCrcLo`        -> `0x5AE0`
- `flash_lookup_15D0`     -> `0x42B0`
- `flash_lookup_15B0`     -> `0x42B8`
- `flash_lookup_1598`     -> `0x42C0`
- `adcMiscSample`         -> `0x2A30`
- `adcVoltageSample`      -> `0x2AFC`
- `adcCurrentSample`      -> `0x2B70`
- `droopTrimCalc`         -> `0x2C32`
- `pidControl`            -> `0x2C78`
- `ocpFoldback`           -> `0x2CD6`

Test levels:

- Simple: helper boundary and latch/debounce vectors (`0x4326`, `0x430C`)
- Medium: ring-buffer/running-sum current average behavior and helper functions in protection/flash (`0x43D4`, `0x30E2`, `0x31B2`, `0x3242`, `0x5B74`, `0x5B86`)
- Medium/Complex: includes ADC sampling and OCP foldback logic (`0x2A30`, `0x2AFC`, `0x2B70`, `0x2C32`, `0x2C78`, `0x2CD6`)
- Complex: startup/overcurrent branch behavior and soft-start branch behavior (`0x433C`, `0x2DF6`, `0x2DC6`, `0x2E2C`)

Current branch coverage goal:

- `decompiled/voltage_loop.c` tested slice: full branch coverage for included unit-test functions.
- `decompiled/pwm.c` tested slice: all included control branches covered.

Type/arity checks:

- Compile-time function-pointer signature checks are included for:
  - `thresholdCompare433C`
  - `latchCounter430C`
  - `adcBuf12OvercurrentLatch`
  - `adcBuf4FastAverage`
  - `softStartRamp2`
