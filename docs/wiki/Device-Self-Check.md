# Device Self-Check

On boot, the firmware can perform a lightweight self-check against the status gateway to determine whether a firmware update is required or recommended.

## Endpoint

- Base URL: `https://status.rpc.gateway.tribewarez.com`
- Path: `/api/device/self-check`
- Method: `GET`

## Query Parameters

| Parameter | Type | Description | Example |
|-----------|------|-------------|---------|
| device_type | string | Device family | `esp32` or `esp8266` |
| channel | string | Release channel | `testnet` |
| kind | string | Artifact kind | `firmware` |
| current_version | string | Version compiled into the binary (`POT_O_VERSION`) | `1.0.0` |

## Example Request

```text
GET https://status.rpc.gateway.tribewarez.com/api/device/self-check?device_type=esp32&channel=testnet&kind=firmware&current_version=1.0.1
```

## Handling the Response

If the response reports `update_required` or `update_recommended` and includes an `artifact.download_url`:

1. Download the new firmware image with a small HTTPS client.
2. Verify the `checksum` (if present).
3. Flash the new image according to your OTA update strategy for the device.

## Notes

- The exact OTA mechanism depends on your hardware and bootloader.
- This endpoint gives a single, canonical source of truth for which firmware version to run per device type and channel.
- The current version comes from `POT_O_VERSION` (default `1.0.0`, set in `platformio.ini`).
