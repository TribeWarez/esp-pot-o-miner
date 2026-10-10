# Mining Flow

This page describes the end-to-end mining process implemented in `src/main.cpp`.

## Endpoints Used

| Method | Path | Purpose |
|--------|------|---------|
| POST | /devices/register | Register the device (by MAC address) |
| GET | /health | Heartbeat / health check |
| POST | /challenge | Fetch a tensor challenge |
| POST | /devices/progress | Report live mining progress (recommended) |
| POST | /submit | Submit a proof for verification |

## Flow

1. **Boot** — Connect to WiFi (`wifi_connect`), then register the device (`register_device`) via `POST /devices/register` with `device_type` and `device_id` (MAC address).

2. **Heartbeat** — Every `HEARTBEAT_INTERVAL_MS` (default 30s), call `GET /health`. If the RPC is unreachable, wait 5s and retry next cycle.

3. **Fetch challenge** — `POST /challenge` with `{"device_type":"esp32s"|"esp8266"}`. The response contains `id`, `operation_type`, `max_tensor_dim`, `difficulty`, `mml_threshold`, `path_distance_max`, `input_tensor` (`data.F32` and `shape.dims`), and `slot_hash`.

4. **Check operation support** — Map the operation name via `op_from_name` and skip if `op_supported` returns false for this device.

5. **Execute tensor operation** — Load the input tensor into `g_input_buf`, clamp to `MAX_TENSOR_DIM`, then run `tensor_execute`. Kernel time is measured with `micros()`.

6. **Compute MML score** — `compute_mml_score` over input and output. If the score exceeds `mml_threshold`, the challenge is abandoned (no point iterating nonces).

7. **Compute expected neural path** — `expected_path(challenge_id, ...)`.

8. **Nonce search** — For `nonce` in `0..MAX_MINE_ITERATIONS`:
   - `compute_actual_path(g_output_buf, out_len, nonce, ...)`
   - `hamming_distance(actual_path, exp_path)`; if `dist <= path_distance_max`, a proof is found.

9. **Assemble proof** — On a match, build the proof JSON:
   - `challenge_id`, `challenge_hash` (from `slot_hash`), `tensor_result_hash`
   - `mml_score`, `path_signature`, `path_distance`, `computation_nonce`
   - `computation_hash` (SHA-256, see below), `miner_pubkey`, `timestamp`

10. **Submit** — `POST /submit` with `{"proof": {...}}`. The response's `accepted` flag and `tx_signature` are logged.

11. **Progress (recommended)** — While mining, periodically `POST /devices/progress` with `challenge_id`, `hash`, and `device_id` so the status dashboard shows live activity.

12. **Stats** — After each challenge, log challenges fetched, proofs found/submitted/accepted, and uptime.

## Proof Hash

`compute_proof_hash` must match `PotOConsensus::compute_proof_hash` on the validator exactly:

```
SHA256(challenge_id_bytes || tensor_hash_bytes || mml_score_le || path_sig_bytes || nonce_le)
```

- `mml_score` is hashed as little-endian `f64` (8 bytes)
- `nonce` is hashed as little-endian `u64` (8 bytes)
- `tensor_hash` is `SHA256(f32 LE data || rows as u64 LE || cols as u64 LE)`

## Serial Output

Key log lines:

```text
[MINE] Challenge: op=relu dim=64 difficulty=2
[MINE] Tensor op=relu rows=64 cols=64 in_len=4096 out_len=4096 op_time_us=...
[MINE] MML score=0.8123 threshold=0.8500 PASS
[MINE] Found proof at nonce=1234 dist=3 mine_time_us=...
[SUBMIT] tx_signature: ...
[STATS] challenges=1 found=1 submitted=1 accepted=1 uptime=42s
```
