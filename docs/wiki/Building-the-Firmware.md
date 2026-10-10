# Building the Firmware

## Prerequisites

- [PlatformIO](https://platformio.org/install/cli) (CLI or IDE extension)
- A USB cable connected to the ESP device
- The ESP32-S or ESP8266 toolchain (installed automatically by PlatformIO on first build)

## Clone and Build

```bash
git clone https://github.com/TribeWarez/esp-pot-o-miner.git
cd esp-pot-o-miner
```

### ESP32-S

```bash
pio run -e esp32s -t upload
```

### ESP8266 (NodeMCU v2)

```bash
pio run -e esp8266 -t upload
```

### Monitor Serial Output

```bash
pio device monitor -b 115200
```

## Build Environments

| Env | Platform | Board | Max Tensor | Working Memory |
|-----|----------|-------|-----------|----------------|
| esp32s | espressif32 | esp32dev | 64×64 | 320 KB |
| esp8266 | espressif8266 | nodemcuv2 | 32×32 | 80 KB |

`default_envs = esp32s` in `platformio.ini`; select another with `-e <env>`.

## Optional Build Flags

Defined in `platformio.ini` and `lib/pot_o_firmware_core/README.md`:

| Flag | Effect |
|------|--------|
| `USE_ESP_DSP` | ESP-DSP–accelerated tiled matmul and dot product on ESP32-S |
| `USE_FAST_ACTIVATIONS` | Fast sigmoid/tanh approximations in tensor ops |
| `USE_SIN_LUT` | Use a sin lookup table in the neural path (server must use the same LUT) |
| `USE_ASM_KERNELS` | Optional Xtensa ASM kernels (matmul, Hamming) when implemented |

Enable them by uncommenting the corresponding line in the `[env:...]` section of `platformio.ini`.

## Verifying the Build

CI builds both environments on every push:

- Workflow: `.github/workflows/ci.yml`
- Status: [github.com/TribeWarez/esp-pot-o-miner/actions](https://github.com/TribeWarez/esp-pot-o-miner/actions)

## Hardware Acceleration

Hardware SHA on ESP32 is enabled via `sdkconfig.defaults` (`CONFIG_MBEDTLS_HARDWARE_SHA=y`).
