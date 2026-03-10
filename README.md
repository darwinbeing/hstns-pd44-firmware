# HSTNS-PD44 Reverse Engineering — Secondary MCU Firmware

> **Reverse-engineered & re-implemented firmware for the HP HSTNS-PD44 PSU secondary-side MCU (dsPIC33FJ64GS606), based on Microchip's 200 W LLC Resonant Converter reference design. Suitable for use as a development board.**

---

## Overview

The **HP HSTNS-PD44** is a high-density server power supply unit. Its secondary side is managed by a **Microchip dsPIC33FJ64GS606** — a high-performance Digital Signal Controller (DSC) specifically designed for digitally-controlled power conversion.

This project reverses the secondary MCU's role, re-implements the control algorithm from scratch using **Microchip's AN1336 / 200 W LLC Resonant Converter** reference design as a blueprint, and makes the hardware usable as a standalone **power electronics development board**.

---

## Hardware

| Item | Detail |
|---|---|
| PSU Model | HP HSTNS-PD44 |
| Secondary MCU | dsPIC33FJ64GS606 |
| Topology | LLC Resonant Converter (Full-Bridge) |
| Rated Output | 12 V / ~70 A (server rail) |
| Reference Design | Microchip 200 W LLC Resonant Converter (AN1336) |
| Programming Interface | ICSP (PGC/PGD) — test points exposed on PCB |

### dsPIC33FJ64GS606 Key Features Used

- High-Speed PWM (HSPWM) — complementary pair for half-bridge gate drive
- 10-bit ADC with dedicated S&H for Vout / Iout / temperature sensing
- Hardware-accelerated PID / compensator (DSP MAC)
- CAN / UART for telemetry
- Fault input pins mapped to over-current / over-voltage trip

---

## Firmware Architecture

```
firmware/
├── src/
│   ├── main.c              # Startup, scheduler
│   ├── pwm.c / pwm.h       # HSPWM init, frequency modulation
│   ├── adc.c / adc.h       # ADC sampling, calibration
│   ├── control.c / control.h   # LLC control loop (SR compensator)
│   ├── fault.c / fault.h   # OVP / OCP / OTP fault handling
│   ├── telemetry.c         # UART debug output
│   └── startup.s           # dsPIC reset vector, stack init
├── inc/
│   └── config_bits.h       # Device configuration bits (oscillator, WDT, etc.)
├── Makefile
└── README.md
```

### Control Algorithm

The control loop follows the architecture described in **Microchip AN1336**:

```
         Vout_ref
            │
            ▼
       ┌─────────┐     error     ┌──────────────┐    f_sw
       │  Σ (−)  │──────────────▶│  Compensator │──────────▶  HSPWM
       └─────────┘               │  (Type II/III│
            ▲                    │   SR filter) │
            │                    └──────────────┘
         Vout_ADC
```

- **Regulation variable**: Switching frequency `f_sw` (LLC characteristic — frequency modulation)
- **Compensator**: Discrete-time Type II (PI + lag) implemented with DSP multiply-accumulate
- **Sampling**: Synchronized ADC trigger from PWM period event
- **Soft-start**: Frequency swept from `f_max` down to operating point on enable
- **SR (Synchronous Rectifier)**: Secondary side gate signals derived from HSPWM dead-time calculation

---

## Getting Started

### Prerequisites

- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide) ≥ 6.x
- [XC16 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers) ≥ 2.x
- MPLAB PICkit 4 / ICD 4 (or compatible ICSP programmer)
- [Microchip AN1336 reference materials](https://www.microchip.com/en-us/application-notes/an1336) (recommended reading)

### Build

```bash
git clone https://github.com/<your-handle>/hstns-pd44-firmware.git
cd hstns-pd44-firmware
make          # or open the .X project in MPLAB X
```

### Flash

1. Connect PICkit 4 to the ICSP header (PGC / PGD / MCLR / VDD / GND test points).
2. Apply 5 V logic supply (do **not** apply AC mains until the firmware is verified).
3. Program via MPLAB X or:

```bash
make program
```

### UART Debug Output

UART2 is routed to a test point at **115200 8N1**. Connect a USB–serial adapter to observe:

```
[BOOT] dsPIC33FJ64GS606 LLC Firmware v0.1.0
[ADC ] Vout=12.02V  Iout=0.00A  Temp=32C
[CTRL] fsw=120.4kHz  duty=--  mode=SOFTSTART
[CTRL] fsw=103.7kHz  duty=--  mode=RUNNING
```

---

## Development Board Usage

Once flashed, the PSU can be powered from mains AC and used as:

| Use Case | Notes |
|---|---|
| 12 V bench supply | Up to ~100 A output (server-grade current capacity) |
| LLC converter testbed | Probe gate drive, resonant tank, SR timing |
| dsPIC33 DSC development | Full ICSP access, UART telemetry available |
| Power electronics education | Waveform analysis of resonant converter behavior |

> ⚠️ **Safety Warning**: This unit operates from mains AC voltage. Observe all electrical safety precautions. Primary-side voltages are lethal. Always use appropriate isolation and PPE.

---

## Reverse Engineering Notes

### ICSP Access

ICSP test points (PGC / PGD / MCLR) are accessible on the secondary-side PCB. See [`docs/board_testpoints.md`](docs/board_testpoints.md) for pad locations.

### Original Firmware

The original HP firmware was read-protected. Control algorithm parameters and peripheral configuration were inferred by:

- Oscilloscope probing of gate drive signals (switching frequency, dead-time)
- Measurement of SR timing vs. load
- Cross-referencing with Microchip AN1336 reference schematic

### Known Differences from AN1336

| Parameter | AN1336 Reference | This Implementation |
|---|---|---|
| Power level | 200 W | ~800 W |
| Resonant tank | Reference values | Measured from hardware |
| ADC reference | Internal | External (TBD) |
| SR gate drive | Discrete | Integrated driver IC |

---

## References

- [Microchip AN1336 — 200W LLC Resonant Converter](https://www.microchip.com/en-us/application-notes/an1336)
- [dsPIC33FJ64GS606 Datasheet](https://www.microchip.com/en-us/product/dsPIC33FJ64GS606)
- [dsPIC33F High-Speed PWM Family Reference Manual (DS70329)](https://ww1.microchip.com/downloads/en/DeviceDoc/70329b.pdf)
- Erickson & Maksimovic, *Fundamentals of Power Electronics* — LLC resonant converter analysis

---

## License

This project is released under the **MIT License**. See [`LICENSE`](LICENSE) for details.

> This project is for educational and development purposes. No proprietary HP firmware or trade secrets are distributed. All code is independently written.

---

## Contributing

Issues and PRs welcome. If you have hardware measurements, schematic tracing, or control loop improvements — please share!
