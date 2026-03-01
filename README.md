# Quill

<p align="center">
  <img src="assets/quill-icon.svg" width="128" height="128" alt="Quill icon">
</p>

**Quill** is a lightweight PDF reader for the [reMarkable 2](https://remarkable.com/) tablet, designed to bridge the gap between reading and annotating.

## Why Quill?

The reMarkable 2 ecosystem has a gap: the built-in reader (xochitl) has great stylus annotation but poor PDF navigation, while KOReader has excellent reading features but no stylus annotation. Users must awkwardly switch between the two.

**Quill combines both** — good PDF reading with native stylus annotation — in a single, lightweight app.

## Features

- **Smart view modes**: Fit-to-width, fit-to-page, crop margins, and **column mode** for two-column academic papers
- **Stylus annotation**: Pressure-sensitive freehand pen, highlighter, eraser, and text notes — saved as standard PDF annotations portable to any PDF reader
- **Fast navigation**: Table of contents, internal PDF links, jump-to-page, bookmarks
- **E-ink optimized**: Partial screen refresh, pre-rendered page cache, minimal UI chrome
- **Lightweight**: Built with MuPDF + Qt5, runs well within the rM2's 1GB RAM

## Status

🚧 **Under active development** — not yet ready for use.

## Target Device

- reMarkable 2 (10.3" monochrome E Ink, 1872×1404, 226 PPI)
- ARMv7 / Cortex-A9, 1GB RAM, Linux Codex OS

A future release will target the reMarkable Paper Pro.

## Building

Quill is cross-compiled for the reMarkable 2 using the official SDK. See [docs/building.md](docs/building.md) for setup instructions.

### Quick Start (requires Docker)

```bash
# Build the cross-compilation Docker image
docker build -t quill-build .

# Cross-compile for reMarkable 2
docker run --rm -v $(pwd):/src quill-build

# Deploy to device
./scripts/deploy.sh
```

## Technology

- **PDF engine**: [MuPDF](https://mupdf.com/) (AGPL-3.0)
- **UI framework**: Qt5 Quick/QML + C++
- **Display**: rm2fb framebuffer shim
- **Input**: evdev (stylus + capacitive touch)

## License

AGPL-3.0 — see [LICENSE](LICENSE) for details. (MuPDF dependency requires AGPL-3.0 for distribution.)

## Contributing

Contributions welcome! Please open an issue to discuss before submitting large changes.

---

*Quill is not affiliated with or endorsed by reMarkable AS.*
