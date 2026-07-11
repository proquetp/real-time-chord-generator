# Real-Time Chord Generator

A real-time musical harmonizer built on a **dsPIC33FJ32MC202 microcontroller**, developed as the final project for the Electronic Systems course at ICAI — Universidad Pontificia Comillas (Master in Industrial Engineering, 2025).

## What it does

The system listens to a note played near a microphone, identifies it by measuring the signal's period, and instantly plays the full chord — root, third and fifth — through three separate speakers simultaneously. A button switches between major and minor chords. A second mode records a short melody and plays it back fully harmonized.

## How it works

| Stage | What it does |
|-------|-------------|
| Microphone circuit | Polarizes an electret mic and captures the raw audio signal |
| Signal conditioning | Buffer + high-pass filter + ×370 amplifier to clean and amplify the signal |
| Note detection | ADC samples the signal; Timer1 measures the period between rising edges (averaged over K=4 cycles) |
| Chord generation | Maps the detected note to the chromatic scale and calculates the third and fifth |
| PWM output | Timer2 ISR generates three independent square waves, one per note, driving the speakers through BJT transistors |

## Modes

- **Automatic (State 0):** live chord generation from microphone input, major or minor selectable via button
- **Melody (State 1):** record up to 9 notes with their durations, then play back the harmonized sequence. Includes a preloaded Tetris theme.

## Hardware

- Microcontroller: dsPIC33FJ32MC202
- Microphone: Electret condenser, 50–16000 Hz
- Signal conditioning: op-amp buffer + non-inverting amplifier (G ≈ 370 V/V)
- Output: 3× piezo buzzer driven by NPN BJT (2N2222A)
- Supply: 3.3V

## Code structure

| File | Description |
|------|-------------|
| `main.c` | Entry point: initializes all modules and runs the main loop |
| `acordes.c/h` | State machine: automatic and melody modes |
| `microfono.c/h` | Note identification from measured period |
| `adc.c/h` | ADC driver with hysteresis for zero-crossing detection |
| `pwm.c/h` | Software PWM via Timer2 ISR, four independent channels |
| `timer3.c/h` | Timer3 driver for tempo/duration counting |
| `config.c/h` | Clock and microcontroller configuration |

## Team

Modesto Álvarez Calvo · Pol Roquet Puigdollers · Pablo Fernández Freige

---
*ICAI — Universidad Pontificia Comillas · Electronic Systems · November 2025*
