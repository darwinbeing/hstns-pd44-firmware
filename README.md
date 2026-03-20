# HSTNS-PD44 800W LLC Firmware Reverse Engineering

Decompiled HP server PSU firmware (dsPIC33FJ64GS606) with AI (Claude) assistance.  
Implemented closed-loop 12.3V output using 2P2Z digital compensator.

## Timer Configuration

```c
// Fcy = 50MHz, period = Fcy / freq - 1
#define T1_PER  10000   // Timer1: 5kHz (200us), slow loop (OCP, droop, ADC filter)
#define T2_PER  1000    // Timer2: 50kHz (20us), 2P2Z compensator + PWM
```

## Architecture

```
TIMER2 ISR (fast, 50kHz)
│
├── ADC: vout_adc = 2×AN0 + 2×AN2
├── Error: e[n] = vout_adc - vref (slew limited ±50, clamped ±100)
├── 2P2Z: y[n] → freq_ctrl
├── PWM:  PDC = PWM_CLK_80 / freq_ctrl >> 1, PTPER = PDC - 8
└── Sub-functions:
    ├── llc_current_fast_avg()    AN4 8-point filter
    ├── llc_voltage_cal_ovp()     OVP protection
    └── fan_speed_update()        fan PWM control

TIMER1 (slow, 5kHz)
│
├── llc_adc_voltage_sample()      dual-channel Vout, 64-point avg
├── llc_adc_current_sample()      Iout, 64+1024 point avg
├── llc_droop_trim_calc()         load-dependent voltage droop
└── llc_ocp_foldback_loop()       OCP/CC PI controller
    └── vref = (0xC2F - trim) - ocp_output → TIMER2

PWM Special Event ISR
└── Atomic PWM register update (PTPER, PDC1/2/3, PHASE3, DTR3)
```

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
| 0x0344 | AN2 | Vout sense B |
| 0x0348 | AN4 | Current sense (fast) |
| 0x034a | AN5 | Vout sense (for OVP and calibration) |
| 0x1da0 | vout_adc | 2×AN0 + 2×AN2 |
| 0x1da4 | freq_ctrl | 2P2Z output → PWM frequency |
| 0x1d4c | vref_2p2z | voltage reference for 2P2Z |
| 0x1d38 | ocp_output | OCP PI output (0-4000) |
| 0x1d3a | trim_offset | droop compensation (0-77) |
| 0x1d44 | Imeas | measured output current |
| 0x1d46 | iout_avg | AN4 8-point average |
| 0x1d66 | vout_cal | AN5 calibrated voltage (for OVP) |

## Hardware

- **MCU**: dsPIC33FJ64GS606, 100MHz with PLL
- **Topology**: LLC resonant converter, push-pull
- **Output**: 12.3V / 800W
- **ADC**: 10-bit, Vout divider 5.23/(5.23+20)
- **PWM**: 3 channels, edge-aligned push-pull mode
