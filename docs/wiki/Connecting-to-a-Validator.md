# Connecting to a Validator

The firmware talks to a PoT-O validator RPC endpoint. This page covers setting up and adjusting that URL.

Default endpoint: `https://pot.rpc.gateway.tribewarez.com`

## How the URL Is Built

Three layers, highest priority first:

1. Build flags in `platformio.ini` (or `--build-flag` on the command line)
2. `#define` fallbacks in `lib/pot_o_firmware_core/include/pot_o/pot_o_config.h`
3. Built-in defaults

At runtime, `src/main.cpp` builds the URL from the macros:

- `POT_RPC_TLS=1` → `https://<POT_RPC_HOST><path>` (port implied 443)
- `POT_RPC_TLS=0` → `http://<POT_RPC_HOST>:<POT_RPC_PORT><path>`

## Changing the Validator URL

### Method A — Edit `platformio.ini`

Change the values in the `[common]` `build_flags` block:

```ini
build_flags =
    -DPOT_RPC_HOST=\"your-host.example.com\"
    -DPOT_RPC_PORT=8900
    -DPOT_RPC_TLS=0
```

### Method B — Pass build flags on the CLI

```bash
pio run -e esp32s -t upload --build-flag="-DPOT_RPC_HOST='\"myhost.example.com\"' -DPOT_RPC_PORT=8900 -DPOT_RPC_TLS=0"
```

### Method C — Edit `pot_o_config.h` directly

Override the `#ifndef` defaults in `lib/pot_o_firmware_core/include/pot_o/pot_o_config.h`:

```c
#define POT_RPC_HOST "myhost.example.com"
#define POT_RPC_PORT 8900
#define POT_RPC_TLS  0
```

## Examples

### Default testnet (HTTPS)

```ini
build_flags =
    -DPOT_RPC_HOST=\"pot.rpc.gateway.tribewarez.com\"
    -DPOT_RPC_PORT=443
    -DPOT_RPC_TLS=1
```

### Local validator (HTTP, custom port)

Run the validator locally (see [pot-o-validator](https://github.com/TribeWarez/pot-o-validator)) and point the device at it:

```ini
build_flags =
    -DPOT_RPC_HOST=\"192.168.1.100\"
    -DPOT_RPC_PORT=8900
    -DPOT_RPC_TLS=0
```

### Custom testnet (HTTPS)

```ini
build_flags =
    -DPOT_RPC_HOST=\"my.validator.example.com\"
    -DPOT_RPC_PORT=443
    -DPOT_RPC_TLS=1
```

## Notes

- When `POT_RPC_TLS=0`, `POT_RPC_PORT` is used; when `POT_RPC_TLS=1`, the port is implied by HTTPS.
- TLS certificate validation is skipped on-device (`setInsecure()`), so the endpoint is trusted by hostname only.
- The device registers itself at `POST /devices/register` and identifies by MAC address.
- After changing the endpoint, rebuild and re-flash the device.
