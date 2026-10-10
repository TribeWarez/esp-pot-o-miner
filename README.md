# ESP PoT-O Miner Firmware

[![CI](https://img.shields.io/github/actions/workflow/status/TribeWarez/esp-pot-o-miner/ci.yml?branch=main)](https://github.com/TribeWarez/esp-pot-o-miner/actions)
[![Latest Tag](https://img.shields.io/github/v/tag/TribeWarez/esp-pot-o-miner?sort=semver)](https://github.com/TribeWarez/esp-pot-o-miner/tags)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

Mining firmware for ESP32-S and ESP8266 microcontrollers.  
Connects to a PoT-O validator RPC endpoint to fetch challenges, execute tensor operations on-device, and submit proofs.

- **Documentation site:** [pot-o-miner.tribewarez.com](https://pot-o-miner.tribewarez.com/)
- **Documentation viewer:** [pot-o-miner.tribewarez.com/docs](https://pot-o-miner.tribewarez.com/docs/)
- **Releases (binaries):** [github.com/TribeWarez/esp-pot-o-miner/releases](https://github.com/TribeWarez/esp-pot-o-miner/releases)
- **Repository:** [github.com/TribeWarez/esp-pot-o-miner](https://github.com/TribeWarez/esp-pot-o-miner)

## Documentation

Full documentation is published at [pot-o-miner.tribewarez.com/docs](https://pot-o-miner.tribewarez.com/docs/) and maintained in the wiki (`docs/wiki/`):

- [Home](docs/wiki/Home.md) — overview and navigation
- [Building the Firmware](docs/wiki/Building-the-Firmware.md)
- [Configuration Reference](docs/wiki/Configuration-Reference.md)
- [Connecting to a Validator](docs/wiki/Connecting-to-a-Validator.md) — setting up and adjusting the validator URL
- [Mining Flow](docs/wiki/Mining-Flow.md)
- [Troubleshooting](docs/wiki/Troubleshooting.md)
- [Contributing Guidelines](docs/wiki/Contributing-Guidelines.md)

## Device Capabilities

| Device   | Max Tensor | Working Memory | Supported Ops |
|----------|-----------|----------------|---------------|
| ESP32-S  | 64×64     | 320 KB         | matrix_multiply, convolution, relu, sigmoid, dot_product, normalize |
| ESP8266  | 32×32     | 80 KB          | relu, sigmoid, dot_product, normalize |

## Quick Start

### Prerequisites

- [PlatformIO](https://platformio.org/install/cli)
- USB cable connected to ESP device

### Configure WiFi

Edit `lib/pot_o_firmware_core/include/pot_o/pot_o_config.h`:

```c
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASS"
```

Or pass via build flags:

```bash
pio run -e esp32s -t upload --build-flag="-DWIFI_SSID='\"MySSID\"' -DWIFI_PASS='\"MyPass\"'"
```

### Build & Upload

```bash
# ESP32-S
pio run -e esp32s -t upload

# ESP8266 (NodeMCU v2)
pio run -e esp8266 -t upload

# Monitor serial output
pio device monitor -b 115200
```

## Mining Flow

1. **Boot** → Connect WiFi → Register device with RPC
2. **Loop**:
   - `POST /challenge` → get tensor challenge
   - Execute tensor operation (matrix multiply, convolution, activation, etc.)
   - Compute MML score (entropy-based approximation of DEFLATE ratio)
   - Search nonces for neural path match within Hamming distance tolerance
   - `POST /submit` → submit proof for on-chain verification
3. **Progress** (recommended): `POST /devices/progress` with `challenge_id`, `hash`, and `device_id` every few seconds while mining so the status dashboard shows live activity.
4. **Heartbeat** every 30s via `GET /health`

For full integration details (endpoints, device_id on submit, progress reporting), see the [Mining Flow](docs/wiki/Mining-Flow.md) wiki page.

## Firmware Updates

The device self-check status gateway is no longer available. For current firmware update guidance, see the [documentation site](https://pot-o-miner.tribewarez.com/docs/).

## Architecture

- **Core library** (reusable across firmware repos): `lib/pot_o_firmware_core/`
  - Tensor ops (base + ESP32 tiled matmul, optional ESP-DSP), neural path, SHA-256, MML score.
  - See `lib/pot_o_firmware_core/README.md` for API and optional build flags (`USE_ESP_DSP`, `USE_FAST_ACTIVATIONS`, `USE_SIN_LUT`).
- **App**: `src/main.cpp` — setup, mining loop, HTTP client, proof assembly. Includes `pot_o/*.h` from the core lib.
- **Legacy redirects**: `include/*.h` redirect to `pot_o/*.h` for backward compatibility.

Hardware SHA on ESP32 is enabled via `sdkconfig.defaults` (`CONFIG_MBEDTLS_HARDWARE_SHA=y`).

## RPC Endpoint

The validator RPC endpoint is set at build time via `POT_RPC_HOST` (see the [Configuration Reference](docs/wiki/Configuration-Reference.md)).

The previous public validator endpoint is no longer available. For the current endpoint, see the [documentation site](https://pot-o-miner.tribewarez.com/docs/).

Override in `platformio.ini` build flags:

```ini
build_flags =
    -DPOT_RPC_HOST=\"your-host.example.com\"
    -DPOT_RPC_PORT=8900
    -DPOT_RPC_TLS=0
```

## License

[MIT](LICENSE)
