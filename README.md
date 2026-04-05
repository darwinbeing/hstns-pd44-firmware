# HSTNS-PD44 Firmware Reverse Engineering Project

## Overview

HSTNS-PD44 is a digital control board from an HP server 800W power supply module based on an LLC series-resonant converter topology. This project extracts the firmware from the physical PCB through hardware reverse engineering, decompiles it into readable C source code, and rebuilds the complete digital power control algorithms in an MPLAB X project.

## Hardware Platform

| Item | Specification |
|------|---------------|
| MCU | Microchip dsPIC33FJ64GS606 (16-bit DSC, 50 MIPS) |
| Main Crystal | 16 MHz external, PLL to Fosc = 100 MHz, Fcy = 50 MHz |
| Auxiliary Clock | FRC 7.37 MHz × 16 = ~120 MHz (dedicated PWM/ADC clock) |
| External Flash | Adesto AT45DB021E 2Mbit SPI DataFlash (JEDEC ID: 1F 23 00) |
| Power Topology | LLC series-resonant full-bridge converter + synchronous rectification |
| Output | 12V / 800W |
| Communication | I2C (slave addr 0x58), UART1 (4800 baud, 9N1), UART2 (115200 baud, 8N1), SPI2 |

## Digital Power Development Platform

The HSTNS-PD44 is a fully digital-controlled 800W LLC resonant converter,
making it an ideal platform for power electronics control algorithm development
and verification:

- **All-digital control**: no analog compensation — 2P2Z coefficients, OCP thresholds,
  soft-start ramp rates are all software-configurable
- **Real hardware**: 800W rated power with production-grade magnetics, MOSFETs, and
  thermal design — not a low-power eval board
- **Rich peripherals**: 10-bit ADC (voltage/current sense), 3-ch PWM with dead-time
  control, Output Compare (fan PWM), UART/I2C (PMBus)
- **Low cost**: widely available used server PSU ($5-15)

### What You Can Experiment With

| Area | Examples |
|------|---------|
| Compensator design | 2P2Z, 3P3Z, PID, state-space, MPC |
| Soft-start | Linear ramp, S-curve, pre-bias handling |
| Current limiting | Constant current, foldback, hiccup mode |
| Load sharing | Droop compensation, active current sharing |
| Adaptive control | Gain scheduling, auto-tuning, neural network |
| Protection | OVP, OCP, OTP with configurable thresholds |
| Frequency sweep | Bode plot measurement via injected perturbation |

## Reverse Engineering Methodology

### 1. Firmware Extraction

A PICkit 5 programmer was used to read the full 64KB program Flash from the dsPIC33F via the ICSP (PGD2) interface, obtaining the complete machine code binary. The on-board AT45DB021E external Flash was also dumped — all 1024 pages (256KB) — via SPI for analysis of calibration parameters and configuration tables.

### 2. Decompilation & Code Reconstruction

The dsPIC33F 24-bit instruction set was disassembled and cross-referenced with Microchip datasheet SFR mappings to reconstruct equivalent C code function by function. Key annotation conventions:

- `DAT_ram_XXXX` — Original decompiler RAM variable address labels
- `FUN_rom_XXXXXX` — Original decompiler ROM function address labels
- `__mulsi3` — dsPIC compiler's internal 32-bit multiplication library function

### 3. Configuration Bits Verification

All configuration bits were read directly from hardware, not guessed. This includes oscillator selection (PRIPLL + XT), watchdog configuration (WDT enabled, PS2048/PR32), comparator polarity, etc., ensuring the recompiled firmware matches the original clock and protection behavior.

## Source Code Structure

```
├── main.c          — Main: peripheral init sequence, PWM release, soft-start voltage ramp, main loop
├── init.c          — Peripheral init: clock PLL, IO ports, PWM (3-ch full-bridge + SR),
│                     ADC, Timer, I2C, UART, SPI, comparator DAC (CMP3/CMP4)
├── isr.c           — Interrupt service routines:
│                     T1 (5kHz): current sampling + droop compensation + OCP foldback
│                     T2 (50kHz): 2P2Z voltage loop compensator + Kff gain scheduling + PWM freq calc
│                     I2C slave interrupt, PWM special event match (register update)
├── i_ctrl.c        — Current measurement & protection:
│                     8-point fast sliding average (TIMER2, 160us window)
│                     64-point mid-term filter (TIMER1, 12.8ms window)
│                     1024-point long-term average (TIMER1, 205ms window)
│                     Dual calibration paths (fast OCP / precise long-term)
│                     PI overcurrent foldback controller (adaptive gain + anti-windup)
├── v_ctrl.c.c      — Voltage measurement & protection:
│                     Dual-channel 64-point voltage sampling + calibration
│                     OVP detection & shutdown (~20us response)
├── at45db.c        — AT45DB021E SPI Flash driver:
│                     Page read/write/erase, JEDEC ID read, binary page size config
│                     UART dump with CRC-16/XMODEM verification
├── serial.c        — UART driver: UART1/UART2 char/string output, debug frame TX
├── traps.c         — CPU exception trap handlers (oscillator fail, address error, stack overflow, math error)
├── define.h        — Global constants: control parameters, fault thresholds, Q15 fixed-point macros, type aliases
└── LLC_VMC.X/      — MPLAB X IDE project files
```
## Control Algorithm Architecture

### Voltage Loop — 2P2Z Digital Compensator (TIMER2, 50kHz)

```
              Vref ──┐
                     ├─ e[n] ── slew rate limit ── amplitude clamp ──┐
   AN0+AN2 ──────────┘                                               │
                                                                      ▼
                                              2P2Z: y[n] = d2·y[n-1] + d3·y[n-2]
                                                         + Kff·(n1·e[n] + n2·e[n-1] + n3·e[n-2])
                                                                      │
                                                               clamp [2600, 25000]
                                                                      │
                                                               u_exec ── ÷ ── PTPER (switching freq)
```

- Frequency control range: 145 kHz – 250 kHz (near LLC resonant frequency)
- Kff feedforward gain: linearly mapped from u_exec, with transient boost (×1.465)
- PWM clock constant: 0xB3FB00 (11,796,224), PTPER = PWM_CLK / u_exec / 2

### Current Loop — PI Overcurrent Foldback (TIMER1, 5kHz)

```
   Imeas (8pt avg) ──┐
                     ├─ i_error ── PI(Kp,Ki) ── anti-windup ── u[n]
        Iref ────────┘                                           │
                                                           ┌─────┘
                                                           ▼
                                              Vref_adj = VREF_NOMINAL - droop - u[n]
                                                           │
                                                           ▼ (fed into TIMER2 voltage loop)
```

- Adaptive gain: PI Kp/Ki scale with output voltage (lower voltage = more aggressive foldback)
- Anti-windup: back-calculation coefficient = 30
- Droop compensation: Imeas × 0.4614, clamped to [0, 77]

### OVP — Over-Voltage Protection (~20us Response)

Independent AN5 sampling path, threshold at 879 counts (~13.9V). Upon detection, all PWM outputs are shut down within 1 TIMER2 cycle.

## 2P2Z Compensator

### Transfer Function

$$H(z) = \frac{-1.952 + 2.425 z^{-1} - 0.691 z^{-2}}{1 - 0.257 z^{-1} + 0.007 z^{-2}}$$

### Coefficients

| Parameter | Float | Q15 Hex | Role |
|-----------|-------|---------|------|
| b0 | -1.95197 | 0xFFFF0626 | e[n] |
| b1 | 2.424896 | 0x00013663 | e[n-1] |
| b2 | -0.69126 | 0xFFFFA785 | e[n-2] |
| a1 | 0.25742 | 0x20F3 | y[n-1] |
| a2 | -0.00742 | 0xFF0D | y[n-2] |

### Zeros and Poles (fs = 50kHz)

| | z | f (fs=50kHz) |
|--|---|-------------|
| Zero 1 | 0.443 | 6.48 kHz |
| Zero 2 | 0.799 | 1.78 kHz |
| Pole 1 | 0.226 | 11.83 kHz |
| Pole 2 | 0.031 | 27.64 kHz |

All poles inside unit circle → stable system.

### Control Bandwidth Analysis

These frequencies describe **how fast the control loop corrects disturbances**,
not the LLC switching frequency (~300kHz).

```
Gain
  │
  │  High gain         Bandwidth        Rolloff
  │  (precise DC)    (fast response)  (noise reject)
  │  ─────────╱─────────────╲──────────
  │          ╱               ╲
  │         ╱                 ╲
  └────────┼────────┼──────────┼──────── f
         1.78k    6.48k     11.83k
         zero2    zero1      pole1
```

| Priority | Parameter | Frequency | Function |
|----------|-----------|-----------|----------|
| 1 | Zero 2 | 1.78 kHz | Low-freq gain boost, eliminates steady-state error |
| 2 | Pole 1 | 11.83 kHz | Bandwidth limit, high-freq attenuation |
| 3 | Zero 1 | 6.48 kHz | Phase boost, increases phase margin |
| 4 | Pole 2 | 27.64 kHz | Switching ripple rejection |

Estimated crossover frequency: **5-10 kHz** (~1/6 of sampling rate), typical for LLC.

### Gain Scheduling

The firmware uses voltage-adaptive gain scaling. The 2P2Z forward path output
is multiplied by a frequency-dependent gain factor before combining with the
feedback path:

```
gain_scale = freq_ctrl × 0x44D >> 15 - 0x4A, clamp [0x2C, 0x80]
gain_scale = gain_scale × freq_gain >> 10

y[n] = ((ff × gain_scale >> 7) + fb) >> 13
```

At lower frequencies (higher load), gain increases → more aggressive regulation.  
At higher frequencies (lighter load), gain decreases → more stability margin.

## PWM Frequency

```
PWM_CLK_80 = 0xB3FB00 (11,795,200)
PDC = PWM_CLK_80 / freq_ctrl >> 1
PTPER = PDC - 8
f_sw = 1 / (2 × PTPER × 1.06ns)
```

| State | freq_ctrl | PTPER | f_sw |
|-------|-----------|-------|------|
| Startup max | 24000 | 237 | ~2 MHz |
| Normal 12.3V | ~3800 | ~1553 | ~304 kHz |
| Min freq (y_min) | 2600 | 2260 | ~209 kHz |

## Soft Start

Ramp `vref` from 0 to nominal (0xC2F). 2P2Z tracks the rising reference,
frequency naturally sweeps from high (low gain) to operating point.

```
vref:  0 ─────────────╱ 0xC2F
Vout:  0V ────────────╱ 12.3V
freq:  24000 ──╲
                ╲──── ~3800
```

Observed freq_ctrl during original firmware startup:

```
24000 → 15896 → 14545 → 14020 → 13162 → 10357 → 4842 → ...
```

Fast initial drop, then gradual convergence — 2P2Z is already computing
during startup, naturally tracking the ramping reference.

Startup OVP: if vout_adc exceeds threshold during ramp, freq_ctrl snaps
back to high frequency (low gain) to prevent overshoot.

## OCP Foldback (PI Controller)

Pure overcurrent protection loop — inactive under normal load, only activates
when Imeas exceeds Iref.

```
e[n] = Imeas - Iref
u[n] = clamp( (I[n-1] + Kp × e[n]) >> 13,  0, 4000 )
I[n] = clamp( I[n-1] + Ki × e[n] + aw[n],  0, 0x01F40000 )

Kp = (gain_scale × 12000) >> 10     (proportional, direct path)
Ki = (gain_scale × 2800) >> 10      (integral, accumulation path)
aw[n] = (u_clamped - u_raw) × 30    (anti-windup)
```

Normal load (Imeas < Iref): u[n] = 0, inactive. 2P2Z regulates alone.  
Overcurrent (Imeas > Iref): u[n] rises sharply (0→4000 in ~10 ADC counts),
pulls down vref → frequency shifts → current limited.

### Voltage-Adaptive Gain

Kp and Ki are modulated by output voltage via `gain_scale`:

```
gain_scale = 0x400 - vout_scaled, clamp [0x100, 0x400]
```

At 12.3V: gain_scale = 0x100 (minimum) → gentle OCP response.  
At lower Vout (droop): gain_scale increases → more aggressive foldback.  
This compensates for LLC's nonlinear gain characteristic at different operating points.

## Droop Compensation

Current-proportional voltage droop for server PSU load sharing:

```c
trim_offset = clamp( Imeas × scale >> 15,  0, 77 )
// scale = 15109 (mode A) or 3850 (mode B)

vref = (0xC2F - trim_offset) - ocp_output
```

As load current increases, trim_offset increases (max 77), slightly lowering
the 2P2Z reference → output voltage droops under load. This ensures even
current sharing between parallel PSUs in N+1 redundant server configurations.

## Hardware Findings

### LATF0 Voltage Trim

LATF0 (RF0 output pin) controls an external resistor network on the voltage
feedback divider. Clearing this pin raises output voltage by ~0.4V:

| LATF0 | Output |
|-------|--------|
| 1 | 11.9V (trimmed down) |
| 0 | 12.3V (nominal target) |

This affects all voltage levels equally (13.9V → 14.4V with same delta),
confirming it modifies the feedback divider ratio, not a digital offset.

## Key RAM Variables

| Address | Name | Description |
|---------|------|-------------|
| 0x0340 | AN0 | Vout sense A (10-bit ADC) |
| 0x0344 | AN2 | Vout sense B (same Vout, dual channel) |
| 0x0348 | AN4 | Current sense (fast) |
| 0x034a | AN5 | Vout sense (for OVP and calibration) |
| 0x1da0 | vfb | 2×AN0 + 2×AN2 (4× oversampled voltage feedback) |
| 0x1da4 | freq_ctrl | 2P2Z output → PWM frequency |
| 0x1d4c | vref_2p2z | voltage reference for 2P2Z |
| 0x1d38 | ocp_adj | OCP PI output (0-4000), offsets vref |
| 0x1d3a | trim_offset | droop compensation (0-77) |
| 0x1d44 | Imeas | measured output current (cal_b, 8pt avg based) |
| 0x1d46 | iout_avg | AN4 8-point average (from TIMER2) |
| 0x1d66 | vout_cal | AN5 calibrated voltage (for OVP) |
| 0x1e4e | Imeas_cal_a | calibrated current (64pt avg based) |
| 0x1d1e | Imeas_longavg | 1024-point long-term current average |
| 0x1d42 | Imeas_scaled | Imeas × 7.625 (for PMBus reporting) |

## ADC Calibration

Calibration coefficients read from SPI Flash at boot:

### Voltage (AN5)
```
cal_coeff  = 0x200F (8207)    → gain = 8207/8192 = 1.0018
cal_offset = 0x0001           → +1 count
vout_cal = AN5 × 1.0018 + 1  (nearly 1:1 mapping)
```

### Current (AN4)
```
cal_a_gain   = 0x2030 (8240)  → gain = 8240/8192 = 1.00586
cal_a_offset = 0xFFF8 (-8)    → -8 counts (zero-point correction)
Imeas = avg × 1.00586 - 8    (zero offset compensates ADC bias at no load)
```

## Current Sensing Pipeline

Three time scales for current measurement, all from the same AN4 input:

```
AN4 (DAT_ram_0348)
  │
  ├──→ TIMER2 (50kHz): iout_buf[8] → iout_avg (DAT_ram_1d46)
  │      └──→ cal_b → Imeas (DAT_ram_1d44)           [for OCP, ~160us window]
  │                     └──→ Imeas_scaled (DAT_ram_1d42) [× 7.625 for PMBus]
  │
  └──→ TIMER1 (5kHz):  ring64_buf[64] → avg64
         └──→ cal_a → Imeas_cal_a (DAT_ram_1e4e)      [~12.8ms window]
                         └──→ ring1k_buf[1024]
                                └──→ Imeas_longavg (DAT_ram_1d1e) [~205ms window]
```

| Stage | Points | ISR | Window | Usage |
|-------|--------|-----|--------|-------|
| Fast | 8 | TIMER2 (50kHz) | 160μs | OCP detection |
| Medium | 64 | TIMER1 (5kHz) | 12.8ms | Mid-term filtering |
| Long | 1024 | TIMER1 (5kHz) | 205ms | PMBus reporting |

## Development Environment

- **IDE**: MPLAB X (Microchip)
- **Compiler**: XC16 (dsPIC/PIC24)
- **Programmer**: PICkit 5
- **Debug Serial**: UART2 @ 115200 baud

## Code Refactoring & Naming Conventions

A comprehensive refactoring has been completed to convert the decompiled assembly-derived code into professional-grade C code with proper function naming following Microchip PSU reference design standards:

### Naming Conventions Applied
- **Functions**: CamelCase (e.g., `state0Idle()`, `adcBuf12OvercurrentLatch()`)
- **Variables**: camelCase with semantic meaning
- **Macros & Constants**: UPPER_CASE (e.g., `ST_IDLE`, `ST_STARTUP`)
- **Consistency**: Unified across all 15+ source files (isr.c, adc.c, pwm.c, protection.c, etc.)

### Refactored Functions (40+ total)
- **State handlers**: `state0Idle()`, `state1Startup()`, `state2Active()`, `state3Fault()`, `state5Handler()`, `state6Handler()`
- **ADC subsystem**: `adcBuf12OvercurrentLatch()`, `adcBuf4FastAverage()`, `adcMiscSample()`, etc.
- **PWM control**: `frequencyLimitControl()`, `pwmDisableAll()`, `pwmOverrideEnable()`
- **Voltage/Current monitoring**: `voutCalibrationAndOvpDetect()`, `portdEdgeFaultDetect()`, `computeFaultBit()`
- **Flash memory**: `flashPageReadVerify()`, `at45dbChipErase()`, `at45dbIsReady()`
- **I2C/PMBus**: `i2cTxAccumulate()`, `i2cBusStuckHandler()`, `pmbusCommandDispatcher()`, `pmbusRxPacketDecode()`
- **System control**: `uartRxTickService()`, `droopMode0Handler()`, `droopMode3Handler()`

### Bug Fixes During Refactoring
- Fixed 40+ broken function declarations (`extern void (void);`)
- Restored 30+ state handler and system function implementations
- Fixed 6 empty state dispatch calls in mainStateDispatch()
- Fixed 4 empty AT45DB ready-check calls in flash operations
- Fixed 1 broken I2C receive handler call
- Removed 2 dangling empty function calls

## Project Status

Core control algorithm decompilation and refactoring is complete, including:
- 2P2Z voltage-mode compensator with Kff gain scheduling
- PI overcurrent foldback controller with adaptive gain and anti-windup
- Multi-stage current/voltage sampling and filtering pipeline
- OVP over-voltage protection logic
- AT45DB021E external Flash driver with proper naming
- Full peripheral initialization configuration
- All functions properly named and documented with assembly cross-references

## TODO

### Completed ✅
- [x] Code decompilation — Assembly → readable C source code
- [x] Function naming standardization — CamelCase following Microchip conventions
- [x] Variable renaming — Semantic names throughout all 15+ files
- [x] Bug fixes — 40+ broken declarations and calls restored
- [x] Documentation — Comments updated with assembly cross-references

### In Progress / Planned
- [ ] OCP/OVP protection — implement overcurrent and overvoltage shutdown with configurable thresholds
- [ ] Fan control — temperature-based fan PWM via Output Compare module
- [ ] SPI Flash read/write — extract and store calibration parameters, coefficients, configuration
- [ ] I2C/PMBus communication — voltage/current/temperature reporting, remote on/off, margin control
- [ ] Firmware checksum — integrity verification at boot, dual firmware redundant backup for fail-safe update

## Contributing

Contributions are welcome! This is an open platform for digital power electronics research and development. Areas where help is especially appreciated:

- **Control algorithms** — implement and benchmark alternative compensators (3P3Z, PID, state-space, MPC, adaptive/neural-network control)
- **Firmware reverse engineering** — decompile additional functions (PMBus handler, PFC stage, standby converter, fault management)
- **Hardware documentation** — schematic tracing, component identification, pinout mapping
- **Testing & validation** — load transient measurements, Bode plots, efficiency curves, thermal characterization

If you have an HSTNS-PD44 unit and a PICkit/ICD programmer, you can start experimenting right away. PRs, issues, and discussions are all welcome.
