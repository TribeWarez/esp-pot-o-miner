# Contributing Guidelines

Contributions are welcome. This repository is maintained by [TribeWarez](https://github.com/TribeWarez).

## How to Contribute

1. Fork the repository and create a branch from `main`.
2. Make your changes.
3. Ensure the project builds with PlatformIO: `pio run`.
4. Open a pull request with a clear description of the change.

## Development Setup

- [PlatformIO](https://platformio.org/install/cli)
- Build both environments locally before opening a PR:

```bash
pio run -e esp32s
pio run -e esp8266
```

## Code Style

- C/C++ (Arduino framework), following the existing style in `src/` and `lib/pot_o_firmware_core/src/`.
- Keep shared logic in `lib/pot_o_firmware_core/`; keep app-specific code in `src/main.cpp`.
- Update the relevant page under `docs/wiki/` when changing configuration or behavior.

## Testing

- `pio run` must succeed for `esp32s` and `esp8266`.
- CI runs on every push (`.github/workflows/ci.yml`).

## Code of Conduct

Be respectful and constructive. We follow a code of conduct aligned with the TribeWarez ecosystem.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
