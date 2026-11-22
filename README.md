# FinTerminal

[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-C06524)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus)](https://isocpp.org/)
[![Qt6](https://img.shields.io/badge/Qt-6.8-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![Python](https://img.shields.io/badge/Python-3.11+-3776AB?logo=python&logoColor=white)](https://www.python.org/)
[![macOS 12+ Monterey](https://img.shields.io/badge/macOS-12.7.6%2B%20Monterey-000000?logo=apple)](https://github.com/selfboss2025-ai/FinTerm)

**FinTerminal** is a completely **free and open-source** native C++20 desktop terminal for financial research, market data, analytics, portfolio management, and AI-assisted workflows — powered by Qt 6 and embedded Python 3.11, with no paid plans, no subscriptions, and no browser/Electron runtime.

---

## Key Features

- **Analytics** — DCF, portfolio optimization, VaR/Sharpe, derivatives pricing, fixed income, alternatives, and an 18-module QuantLib suite
- **AI Workflows (BYOK)** — trader, economic, and geopolitics agents using your own API keys (OpenAI, Anthropic, Gemini, Groq, DeepSeek, OpenRouter, or local Ollama)
- **Data Connectors** — 100+ public & API connectors including FRED, IMF, World Bank, DBnomics, AkShare, Polygon, Kraken, Yahoo Finance, and government data sources
- **Trading & Backtesting** — crypto & equity feeds, paper-trading engine, backtesting, algo trading, and broker integrations
- **Automation & Quant Lab** — visual node editor, MCP tools, ML/factor discovery
- **Global Intelligence** — maritime tracking, geopolitical analysis, and relationship mapping

---

## Build from Source

### macOS 12.7.6 Monterey (Intel `x86_64` & Apple Silicon `arm64`) / Linux

```bash
git clone https://github.com/selfboss2025-ai/FinTerm.git
cd FinTerm
./setup.sh
```

> **macOS 12.7.6 Monterey note:** Apple Command Line Tools on Monterey max out at Apple Clang 14.0. `setup.sh` automatically detects this and uses **Homebrew LLVM** (`brew install llvm`) with `CMAKE_OSX_DEPLOYMENT_TARGET=12.0` and Qt 6.8.3.

### Manual Build (CMake + Ninja)

Required toolchain: **CMake 3.27+ · Ninja · Qt 6.8.3 · Python 3.11+ · Clang 15+ (or Homebrew LLVM on macOS 12) / GCC 12.3+ / MSVC 19.40+**.

```bash
cd fincept-qt
cmake --preset macos-release -DCMAKE_PREFIX_PATH="/path/to/Qt/6.8.3/macos"
cmake --build --preset macos-release
./build/macos-release/FinTerminal.app/Contents/MacOS/FinTerminal
```

For Linux use `--preset linux-release`; for Windows use `--preset win-release`.

---

## License

**AGPL-3.0-or-later** — see [LICENSE](LICENSE) for full details. FinTerminal is 100% free and open source with no paid tiers.
