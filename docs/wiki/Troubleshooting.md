# Troubleshooting

## WiFi Connection Fails

```text
[WIFI] Connection failed, will retry
```

1. Verify `WIFI_SSID` and `WIFI_PASS` (build flags or `pot_o_config.h`).
2. Ensure the network is 2.4 GHz (ESP32-S/ESP8266 do not support 5 GHz).
3. Move the device closer to the access point.

## RPC Unreachable

```text
[HEARTBEAT] RPC unreachable, retrying next cycle
```

1. Verify `POT_RPC_HOST` and that `POT_RPC_TLS` matches the endpoint (see [Connecting to a Validator](Connecting-to-a-Validator)).
2. When using HTTP (`POT_RPC_TLS=0`), confirm `POT_RPC_PORT` is correct.
3. Check that the device's network can reach the validator (DNS, firewall).

## Challenge Fetch Fails

```text
[MINE] Failed to fetch challenge, waiting 10s
```

1. Verify `POST /challenge` is served by the endpoint.
2. Confirm `device_type` in the request body matches your build (`esp32s` or `esp8266`).
3. Check for a JSON parse error in the log (`[CHALLENGE] JSON parse error`).

## Operation Not Supported

```text
[MINE] Operation 'matrix_multiply' not supported on this device, skipping
```

ESP8266 supports only `relu`, `sigmoid`, `dot_product`, and `normalize`. Use an ESP32-S for `matrix_multiply` and `convolution`.

## No Proof Found

```text
[MINE] Exhausted nonces without proof
```

1. Increase `MAX_MINE_ITERATIONS`.
2. The difficulty may be too high — lower it on the validator (see [pot-o-validator](https://github.com/TribeWarez/pot-o-validator)).
3. Verify `NEURAL_LAYER_WIDTHS` match the server exactly (32, 16, 8).

## MML Score Fails

```text
[MINE] MML score=0.9300 threshold=0.8500 FAIL
```

1. Check `MAX_TENSOR_DIM` and `MAX_WORKING_MEM` limits for your device.
2. Confirm the tensor operation matches what the server expects.
3. If using `USE_SIN_LUT` or `USE_FAST_ACTIVATIONS`, ensure the server uses the same settings.

## Proof Rejected

```text
[MINE] Proof rejected or submission failed
```

1. Verify `compute_proof_hash` matches the validator's `PotOConsensus::compute_proof_hash` (see [Mining Flow](Mining-Flow)).
2. Confirm the neural path parameters match the server.
3. Ensure the device clock/timestamp is acceptable to the validator.

## Firmware Self-Check Fails

1. Verify `status.rpc.gateway.tribewarez.com` is reachable from the device.
2. Confirm `device_type`, `channel`, `kind`, and `current_version` query parameters are correct (see [Device Self-Check](Device-Self-Check)).

## Watchdog Reset During Mining

The nonce loop calls `yield()` every 1000 iterations to prevent WDT resets. If resets still occur, reduce `MAX_MINE_ITERATIONS`.

## Getting Help

1. Check GitHub Issues.
2. Open a new issue with the bug report template.
3. Include: device type, firmware version, `platformio.ini` build flags, serial logs, and repro steps.
