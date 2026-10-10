# Configuration Reference

## Configuration Priority

1. Build flags (`platformio.ini` or `--build-flag`)
2. `#define` overrides in `lib/pot_o_firmware_core/include/pot_o/pot_o_config.h`
3. Built-in defaults (compiled into the core library)

Build flags are `-D` defines; because the header uses `#ifndef` guards, any value passed on the build line wins.

## WiFi

| Field | Type | Default | Build Flag |
|-------|------|---------|------------|
| WIFI_SSID | string | "YOUR_SSID" | `-DWIFI_SSID='"MySSID"'` |
| WIFI_PASS | string | "YOUR_PASS" | `-DWIFI_PASS='"MyPass"'` |

## RPC Endpoint

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| POT_RPC_HOST | string | "pot.rpc.gateway.tribewarez.com" | Validator hostname |
| POT_RPC_PORT | int | 443 | HTTP port (used only when `POT_RPC_TLS=0`) |
| POT_RPC_TLS | int | 1 | 1 = HTTPS (port implied 443), 0 = HTTP |

See [Connecting to a Validator](Connecting-to-a-Validator) for how the URL is built and how to change it.

## Mining Parameters

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| MAX_MINE_ITERATIONS | int | 10000 | Maximum nonce search iterations per challenge |
| HEARTBEAT_INTERVAL_MS | int | 30000 | Health-check interval in milliseconds |

## Device Constraints

| Field | Type | ESP32-S | ESP8266 |
|-------|------|---------|---------|
| MAX_TENSOR_DIM | int | 64 | 32 |
| MAX_WORKING_MEM | int | 327680 (320 KB) | 81920 (80 KB) |
| NEURAL_LAYER_WIDTHS | list | {32,16,8} | {32,16,8} |

## Firmware Version

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| POT_O_VERSION | string | "1.0.0" | Firmware version string, used by the self-check endpoint |

## Neural Path

The neural path shape must match the server exactly (32, 16, 8 = 56 neurons). These are fixed in `pot_o_config.h`:

| Macro | Value |
|-------|-------|
| NEURAL_LAYERS | 3 |
| NEURAL_LAYER_0 | 32 |
| NEURAL_LAYER_1 | 16 |
| NEURAL_LAYER_2 | 8 |
| NEURAL_TOTAL_NEURONS | 56 |

## URL Construction

The runtime URL is built in `src/main.cpp` (`rpc_url`):

```cpp
String rpc_url(const char* path) {
#if POT_RPC_TLS
    return String("https://") + POT_RPC_HOST + path;
#else
    return String("http://") + POT_RPC_HOST + ":" + String(POT_RPC_PORT) + path;
#endif
}
```

When `POT_RPC_TLS=1`, the URL is `https://<host><path>` (port implied 443).
When `POT_RPC_TLS=0`, the URL is `http://<host>:<port><path>`.
