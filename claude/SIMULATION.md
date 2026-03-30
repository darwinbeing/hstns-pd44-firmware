# HSTNS-PD44 MPLAB X Simulator Usage

## Goal

Use the MPLAB X built-in simulator to single-step the decompiled control algorithm without relying on real ADC, PWM, SPI Flash, or board-side startup events.

## What Was Added

- `decompiled/simulator.c`
- `decompiled/simulator.h`
- `SIMULATION_MODE` integration in `decompiled/main.c`
- `SIMULATION_MODE` clock-init bypass in `decompiled/init.c`
- `SIMULATION_MODE` flash-service bypass in `decompiled/state_machine.c`
- `SIM_AUTOMATION_MODE` stop-and-hold path for command-line regression

When `SIMULATION_MODE` is enabled:

- `main()` stops calling the flash/SPI maintenance path that can block in simulator.
- A software-driven loop injects ADC samples and synthetic state transitions.
- `_T1Interrupt()`, `_T2Interrupt()`, and `_PWMSpEventMatchInterrupt()` are called directly.
- `mainStateDispatch()` is triggered periodically by software.
- flash calibration/readback background services are skipped so simulator execution does not block on SPI Flash polling.
- Key internal variables are mirrored into `sim_debug` for watch-window inspection.
- History data is sampled on host side by `mdb` (default), so MCU RAM is not used for long trace buffers.

## Enable In MPLAB X

1. Open project `HSTNS-PD44.X`.
2. Open project properties.
3. Select the simulator as the debugger/tool.
4. Add preprocessor macro `SIMULATION_MODE` for the configuration you use.
5. Clean and build.
6. Start debug.

For manual MPLAB X debug, define only `SIMULATION_MODE`.
For command-line automation, use `make sim`, which adds `SIM_AUTOMATION_MODE` automatically.

## Command-Line Automation

Run:

- `make sim`
- `make sim SIM_TEST_TARGET_STEPS=16`
- `make sim SIM_WAIT_SECONDS=4`
- `make sim SIM_HOST_SAMPLES=1000 SIM_SAMPLE_WAIT_MS=1`
- `make sim-t2`  (default 1500 steps; observe `llcPeriodCmd` convergence)
- `make sim-2p2z`  (pure algorithm model; does not run MCU interrupts/state machine)

The automation flow is:

- clean rebuild with `SIMULATION_MODE` and `SIM_AUTOMATION_MODE`
- run MPLAB Simulator through `mdb.sh`
- repeat `Run -> Wait -> Halt -> Print` to sample key variables into host CSV
- generate `out/sim/sim_trace.csv` and `out/sim/sim_trace.svg`
- return process exit code `0` on pass, non-zero on failure

Sampling knobs:

- `SIM_HOST_SAMPLES`: host-side sample count (default `200`)
- `SIM_SAMPLE_WAIT_MS`: wait between samples in `mdb Wait` units (default `5`)
- `SIM_POST_HALT_WAIT_MS`: extra `mdb Wait` right after `Halt` for symbol-sync stability (default `1`)
- `SIM_MDB_RETRIES`: auto-retry count when `mdb` log misses key symbols (default `6`)
- `SIM_INCLUDE_INITIAL_SAMPLE`: include one pre-run sample point (default `1`)
- `SIM_TEST_TARGET_STEPS`: firmware-side simulation stop step

`make sim-t2` defaults:

- `SIM_T2_TARGET_STEPS=1500`
- `SIM_T2_HOST_SAMPLES=500`
- `SIM_T2_SAMPLE_WAIT_MS=1`

For longer T2 settling observation:

- `make sim-t2 SIM_T2_TARGET_STEPS=6000 SIM_T2_HOST_SAMPLES=1600 SIM_T2_SAMPLE_WAIT_MS=1`
- output files: `out/sim/sim_trace.csv` and `out/sim/sim_trace.svg`

Expected trend in T2-only closed loop:

- `llcPeriodCmd` starts near `24000`, then decreases and should settle roughly in `3000..4000`.
- `AN0/AN2` start from low code then ramp to steady feedback.
- `voutSetpoint` should not collapse after steady state (watch around expected code e.g. `0xC2F` if your calibration path uses that target).

Startup state-machine validation in `make sim`:

- use summary fields in console output:
  - `state_mask` (bit0=IDLE, bit1=STARTUP, bit2=ACTIVE)
  - `idle_to_startup`
  - `startup_to_active`
- pass criteria in chain mode requires:
  - IDLE/STARTUP/ACTIVE all seen
  - `idle_to_startup >= 1`
  - `startup_to_active >= 1`
- this avoids missing short `STARTUP` residency due host sampling interval.

RAM policy:

- `SIM_ENABLE_TARGET_TRACE=0` by default, i.e. no large `sim_trace_*` arrays in MCU RAM.
- For dsPIC33FJ64GS606 (9KB RAM), this avoids RAM exhaustion during long simulations (e.g. 1s windows).

## Recommended Watches

Add these expressions to the Watches window:

- `sim_debug`
- `systemState`
- `statusFlags`
- `faultFlags`
- `voutSetpoint`
- `voutReference`
- `voltageError`
- `Imeas`
- `compY_out`
- `ptper_computed`
- `pdc1`
- `OC2RS`

## Recommended Breakpoints

- `decompiled/main.c:146`
- `decompiled/isr_t1.c:163`
- `decompiled/isr_t2.c:63`
- `decompiled/monitoring.c:96`
- `decompiled/state_machine.c:186`

## How To Drive Test Cases

Default mode uses an internal stimulus profile:

- startup ramp
- normal regulation
- heavier load
- droop / overcurrent stress

You can switch to manual injection during debug:

1. Set `sim_debug.auto_stimulus = 0`
2. Modify these watch variables before continuing:
- `sim_debug.adcbuf0`
- `sim_debug.adcbuf2`
- `sim_debug.adcbuf4`
- `sim_debug.adcbuf5`
- `sim_debug.adcbuf12`
- `sim_debug.adcbuf14`

Timing can also be adjusted live:

- `sim_debug.t2_divider`
- `sim_debug.t4_divider`

## Notes

- This is algorithm-oriented simulation, not peripheral-accuracy simulation.
- The simulator path forces `fw_mode = 2` so the normal control segment is exercised.
- Calibration coefficients are initialized to unity-equivalent defaults inside `simInit()`.

## Troubleshooting

If you see `Simulation log did not contain complete result symbols (mdb_rc=255)`:

1. Run with log dump enabled:
   - `SIM_PRINT_MDB_LOG=1 make sim-t2`
2. Increase retry and sync waits:
   - `SIM_MDB_RETRIES=10 SIM_SAMPLE_WAIT_MS=2 SIM_POST_HALT_WAIT_MS=2 make sim-t2`
3. Reduce sampling pressure for slow simulator sessions:
   - `SIM_T2_HOST_SAMPLES=400 make sim-t2`
4. Confirm `mdb.sh` path is valid:
   - `/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh`

If you see `ValueError: could not convert string to float: ''`:

- This usually means an older plot parser read empty fields in CSV.
- Current `scripts/plot_sim_trace.py` already tolerates empty cells; rerun with latest script:
  - `make sim-t2`
- If needed, remove stale output and rerun:
  - `rm -rf out/sim && make sim-t2`

Memory strategy for long simulation windows:

- Default flow samples via `mdb Print` on host side (`SIM_ENABLE_TARGET_TRACE=0`), so MCU RAM is not consumed by long trace buffers.
- Prefer increasing `SIM_T2_HOST_SAMPLES` instead of enabling large target arrays on dsPIC33FJ64GS606 (9KB RAM).
