# Current C Unit-Test Scope

This unit-test target currently covers the following decompiled functions directly in C:

- `decompiled/voltage_loop.c`
  - `thresholdCompare433C` (`0x4326`)
  - `latchCounter430C` (`0x430C`)
  - `adcBuf12OvercurrentLatch` (`0x433C`)
  - `adcBuf4FastAverage` (`0x43D4`)
- `decompiled/pwm.c`
  - `softStartRamp` (`0x2DC6`)
  - `softStartRamp2` (`0x2DF6`)
  - `stateControlMachine` (`0x2E2C`)
- `decompiled/protection.c`
  - `thresholdCompare` (`0x30E2`)
  - `debounceCounter` (`0x31B2`)
  - `clampToward` (`0x3242`)
- `decompiled/flash.c`
  - `crc16Update` (`0x5B74`)
  - `crc16Block` (`0x5B86`)
  - `eepromSetConfig` (`0x5A88`)
  - `eepromSetCrcLo` (`0x5ACC`)
  - `eepromSetPageAddr` (`0x5AD4`)
  - `eepromGetCrcLo` (`0x5AE0`)
  - `flash_lookup_15D0` (`0x42B0`)
  - `flash_lookup_15B0` (`0x42B8`)
  - `flash_lookup_1598` (`0x42C0`)
- `decompiled/adc.c`
  - `adcMiscSample` (`0x2A30`)
  - `adcVoltageSample` (`0x2AFC`)
  - `adcCurrentSample` (`0x2B70`)
  - `droopTrimCalc` (`0x2C32`)
  - `pidControl` (`0x2C78`)
  - `ocpFoldback` (`0x2CD6`)

For this tested slice, `make c-unittest-cov` reports:

- `decompiled/voltage_loop.c` line/branch/function coverage at 100% for included unit-test functions
- `decompiled/protection.c` helper-function slice at 100% line/branch/function coverage
- `decompiled/pwm.c` included slice with full branch coverage

Notes:

- `UNIT_TEST_MINIMAL` is used to isolate deterministic logic for host-side C unit tests.
- Integration/state-machine behavior remains covered by simulator automation targets (`make sim`, `make sim-t2`).
