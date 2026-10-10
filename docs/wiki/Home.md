# ESP PoT-O Miner Wiki

Welcome to the ESP PoT-O Miner firmware documentation.

## Quick Navigation

- [Building the Firmware](Building-the-Firmware)
- [Configuration Reference](Configuration-Reference)
- [Connecting to a Validator](Connecting-to-a-Validator)
- [Mining Flow](Mining-Flow)
- [Troubleshooting](Troubleshooting)
- [Contributing Guidelines](Contributing-Guidelines)

## What is the ESP PoT-O Miner?

Mining firmware for ESP32-S and ESP8266 microcontrollers. It connects to a PoT-O (Proof of Tensor Optimizations) validator RPC endpoint, fetches tensor challenges, executes tensor operations on-device, computes MML scores, searches for neural path matches via nonce iteration, and submits proofs for on-chain verification.

The validator RPC endpoint is configured at build time. The previous public validator endpoint is no longer available — see the [documentation site](https://pot-o-miner.tribewarez.com/docs/) for the current endpoint.

- **Documentation site:** [pot-o-miner.tribewarez.com](https://pot-o-miner.tribewarez.com/)
- **Documentation viewer:** [pot-o-miner.tribewarez.com/docs](https://pot-o-miner.tribewarez.com/docs/)
- **Releases (binaries):** [github.com/TribeWarez/esp-pot-o-miner/releases](https://github.com/TribeWarez/esp-pot-o-miner/releases)
- **Repository:** [github.com/TribeWarez/esp-pot-o-miner](https://github.com/TribeWarez/esp-pot-o-miner)
- **Validator:** [github.com/TribeWarez/pot-o-validator](https://github.com/TribeWarez/pot-o-validator)

## Device Capabilities

| Device   | Max Tensor | Working Memory | Supported Ops |
|----------|-----------|----------------|---------------|
| ESP32-S  | 64×64     | 320 KB         | matrix_multiply, convolution, relu, sigmoid, dot_product, normalize |
| ESP8266  | 32×32     | 80 KB          | relu, sigmoid, dot_product, normalize |

## Getting Help

- GitHub Issues — Bug reports and feature requests
- GitHub Discussions — Community Q&A
- Security Advisories — Report vulnerabilities (see [SECURITY.md](https://github.com/TribeWarez/esp-pot-o-miner/blob/main/SECURITY.md))
