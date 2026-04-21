# pzep-tui
## A Terminal-Based Vim-Like Editor

A standalone terminal UI implementation of the pZep editor. Renders in terminal using ANSI escape codes.

---

## Features

- Full Vim emulation (motion, operators, ex commands)
- Terminal renderer (ANSI colors)
- Multiple file editing with tabs/splits
- Syntax highlighting
- Git integration
- Macros and folds

---

## Building

### Windows
```powershell
cd platforms/build-win
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Linux
```bash
cd platforms/build-lin
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### FreeBSD
```bash
cd platforms/build-bsd
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Running

```bash
./pzep-tui file.txt
```

---

## Keybindings

See main pZep documentation for full Vim keybinding reference.