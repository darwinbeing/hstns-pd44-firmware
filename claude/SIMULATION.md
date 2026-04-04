# HSTNS-PD44 MPLAB X Simulator Usage (SCL Only)

## Goal

Use MPLAB X simulator to run unmodified firmware, inject external hardware signals with Stimulus Control Language (SCL), and observe firmware variables.

## Current Approach

- No `SIMULATION_MODE` firmware hooks.
- Firmware performs normal initialization flow.
- Timer interrupts are driven by simulator time progression (`Run/Wait`), not by manual ISR calls.
- External hardware behavior is injected through SCL (`stim ...`).
- Trace sampling is done on host side using MDB `Print`.

## Commands

- `make sim` (alias of `make sim-scl`)
- `make sim-scl`

Common knobs:

- `SCL_FILE` (default `scripts/stimulus/hstns_startup_llc.scl`)
- `SCL_HOST_SAMPLES` (default `200`)
- `SCL_SAMPLE_WAIT_MS` (default `2`)
- `SCL_POST_HALT_WAIT_MS` (default `1`)
- `SCL_INIT_WAIT_MS` (default `2`)
- `SCL_WARMUP_CYCLES` (default `1`)
- `SCL_FLASH_DUMP` (optional, AT45 dump bin path; injects calibration coefficients into `cal_*` variables after warmup)

Example:

```bash
make sim-scl SCL_HOST_SAMPLES=400 SCL_SAMPLE_WAIT_MS=1
```

## Outputs

- `out/sim_scl/hstns_pd44_scl_mdb.log`
- `out/sim_scl/scl_trace.csv`

## Recommended Watch Variables

- `systemState`
- `llcPeriodCmd`
- `voutSetpoint`
- `vout_sum`
- `marginThreshold`
- `auxFlags`
- `statusFlags`
- `runtimeFlags`
- `startupFlags`
- `ADCBUF0`, `ADCBUF2`, `ADCBUF12`
- `PTPER`, `OC2RS`

## Troubleshooting

If symbols are missing in the log:

1. Increase retries/waits:
   - `SCL_MDB_RETRIES=20`
   - `SCL_SAMPLE_WAIT_MS=2`
   - `SCL_POST_HALT_WAIT_MS=2`
2. Enable raw MDB log:
   - `SCL_PRINT_MDB_LOG=1 make sim-scl`
3. Verify `mdb.sh` path:
   - `/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh`

## Latest Finding (2026-04-04): `droopKffFactor`

- In current decompiled build, `droopKffFactor` does **not** have a static initializer.
- Original ASM writes `droopKffFactor` in T2 ISR transient branches (set `0x5DC` / restore `0x400`), but those branches require preconditions (`statusFlags` bit9/bit10, `runtimeFlags` bit8/bit9).
- In no-init simulation, `llcPeriodCmd` can stay at `24000`, so bit9/bit10 conditions are never reached, and `droopKffFactor` remains `0`.

Observed no-init behavior:

- `droopKffFactor = 0` (entire run)
- `llcPeriodCmd = 24000` (entire run)
- `statusFlags.bit11` may set, but `statusFlags.bit9/bit10` remain clear
- therefore Kff transient branches are not entered

Important for MDB/SCL injection:

- Use **symbol address from current ELF map**, not the original firmware RAM address.
- In this build, `_droopKffFactor` is at `0x099E` (map), while original firmware analysis references `0x1DA6`.
- Writing `0x1DA6` in simulator does not affect this build variable; writing `0x099E` does.

Example one-shot MDB seed command:

```mdb
write /rh 0x099E 0x0400
```
