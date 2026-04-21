# pZep
## A Standalone Vim-Like Editor

A lightweight, embeddable Vim-like text editor with full Vim emulation.

---

## Structure

```
pzep/
  pzep-tui/      # Terminal UI version
  pzep-gui/      # GUI (ImGui) version
  platforms/    # Build configurations
    build-win/  # Windows build
    build-lin/   # Linux build
    build-bsd/   # FreeBSD build
```

---

## Quick Start

### Build for your platform

**Windows:**
```powershell
cd platforms/build-win
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

**Linux:**
```bash
cd platforms/build-lin
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

**FreeBSD:**
```bash
cd platforms/build-bsd
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Versions

### pzep-tui
Terminal-based rendering using ANSI escape codes.

### pzep-gui  
GUI rendering using ImGui.

---

## Features

- Full Vim emulation (~75% compatible)
- Ex commands (~30% implemented)
- Multiple file editing (tabs/splits)
- Syntax highlighting
- Git integration
- Macros, folds, visual mode
- Multiple cursors
- Minimap

---

## Documentation

- [Vim Compatibility Report](docs/VIM_COMPATIBILITY_REPORT.md)
- [Vim Commands Report](docs/VIM_COMMANDS_REPORT.md)