# Sigfox Protocol Physical Layer for S2-LP - Demo for ESP32

`renard-phy-s2lp-demo-esp32` is an ESP32 demo application for the open source Sigfox PHY layer [`renard-phy-s2lp`](https://github.com/Jeija/renard-phy-s2lp), which interfaces with STMicroelectronics' S2-LP ultra-low power transceiver chip.

It combines [`renard-phy-s2lp`](https://github.com/Jeija/renard-phy-s2lp) and renard-phy-s2lp's ESP32 hardware abstraction layer [`renard-phy-s2lp-hal-esp32`](https://github.com/Jeija/renard-phy-s2lp-hal-esp32).

## Building
### Clone
Make sure to clone this repository recursively so that all submodules get downloaded as well:
```
git clone --recursive https://github.com/Jeija/renard-phy-s2lp-demo-esp32/
```

If you forgot `--recursive` while cloning, you can also use the following command to initialize all submodules:
```
git submodule update --init --recursive
```

### Configure
See [`renard-phy-s2lp-hal-esp32`](https://github.com/Jeija/renard-phy-s2lp-hal-esp32) for information on the default pin configuration and on how to change it.

Edit `main/main.c` to set your Sigfox credentials (device ID, Network Authentication Key and payload).

### Compile
Make sure you have [esp-idf](https://github.com/espressif/esp-idf) installed and your `PATH` is set up correctly. Just use the normal [esp-idf](https://github.com/espressif/esp-idf) workflow to compile and flash this application:

```
idf.py all
idf.py flash
```

## Usage
`renard-phy-s2lp-demo-esp32` transmits a Sigfox uplink directly after the ESP32 has started and the S2-LP has been detected. It then waits for a downlink and outputs the received downlink frame on the serial console. Watch the serial console for debug output.

## Licensing
`renard-phy-s2lp-demo-32` is licensed under the MIT License. See `LICENSE` for details.