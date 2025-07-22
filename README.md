# Vicious Cycle

This project contains Arduino sketches and supporting code used to experiment with measuring bicycle speed using an IR sensor.  It is based on my experience making a system for [using my exercise bike as a game controller](https://joereddington.com/2025/03/09/advanced-spin-bike-speedometer.html). 

The custom design detects 1/60th of a wheel rotation using an IR reflective sensor and a flywheel covered in alternating black and white segments.  This allows a stop or start to be recognised in roughly 0.1&nbsp;seconds compared with the several seconds lag of a consumer cadence sensor.

The code is written to act as a game controller and is currently hardcoded for *Road Rash 2* played via a Retrode and OpenEmu.

The `speed` folder holds the main logic while `sensorChecking` contains a standalone sketch for analysing raw sensor readings.  There are also design assets for the flywheel design that the sensor reads and a small C++ test harness that builds on a desktop without Arduino libraries.

## Hardware

| Item | Price | Quantity Bought | Quantity Needed |
|---|---|---|---|
| [Arduino Leonardo](https://www.amazon.co.uk/dp/B0827DHT3B?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1) | £11.99 | 1 | 1 |
| [I2C IIC LCD 1602 Module](https://www.amazon.co.uk/dp/B0B76Z83Y4?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_5&th=1) | £10.95 | 2 | 1 |
| [TCRT5000 IR Reflective Sensor](https://www.amazon.co.uk/dp/B07L3NRTF7?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1) | £8.99 | 10 | 1 |
| Magnetic segment pattern printing | £20.00 | 1 | 1 |
| **Total** | **£51.93** | | |
Additional equipment such as basic wood for a stand, wires, and either a crimping kit or a soldering iron are required for assembly.  The sensor housing and display bezel were 3D printed and an Ethernet cable was repurposed for the long run between them.



## Directory overview

- **speed/** – primary Arduino sketch and helper code for computing cadence, speed and distance.  The logic is separated in `bike_logic.*` for easier testing.
- **sensorChecking/** – sketch that logs detailed statistics from the IR sensor so that thresholds can be tuned.
- **make_flywheel.py** – Python script that creates `design.png`, an alternating black/white wheel pattern used for calibration.
- **test_bike_logic.desktop.cpp** – simple desktop program that exercises the logic without requiring an Arduino board.
- **circuit diagram.pdf** – schematic of the hardware setup.

## Development history
[More details here](https://joereddington.com/2025/03/09/advanced-spin-bike-speedometer.html)
Early prototypes used a photoresistor and later an IR sensor mounted to a wooden stand. A 3D printed cover and LCD enclosure were added in version 2 and the electronics were tidied up using a crimping kit.
Version 2 also rebuilt the stand so that a laptop and Retrode fit easily on the desk. Road Rash 2 is used as the test game via RetroArch: when cadence rises above 80 the accelerator is held automatically. This highlighted how normal cadence sensors only send data roughly every 0.9 seconds which is far too slow for gaming.
Version 3 moved the code to an Arduino Leonardo so that the device could emulate a USB keyboard for game control and included a small diagnostic program to check sensor accuracy.

Future improvements include cycling different metrics on the display, a custom housing that mounts directly to the bike, a simple speaker that warns when cadence drops, and experimenting with more than sixty wheel segments for even higher precision.

## Running the desktop tests

The C++ test file can be compiled with a standard compiler:

```bash
g++ test_bike_logic.desktop.cpp -o test_bike_logic
./test_bike_logic
```

## License

This project is released under the [Creative Commons Attribution-NonCommercial 4.0 International](https://creativecommons.org/licenses/by-nc/4.0/) license.  You are free to share and adapt the material for non‑commercial purposes as long as you give appropriate credit and indicate if changes were made.

