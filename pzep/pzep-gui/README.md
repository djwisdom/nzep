# pzep-gui
## A GUI-Based Vim-Like Editor

A standalone GUI implementation of the pZep editor using ImGui for rendering.

---

## Features

- Full Vim emulation (motion, operators, ex commands)
- ImGui renderer with menus and dialogs
- Multiple file editing with tabs/splits
- Syntax highlighting
- Git integration
- Macros and folds
- Multiple cursors
- Minimap

---

## Building

### Windows
```powershell
cd platforms/build-win
cmake .. -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Linux
```bash
cd platforms/build-lin
cmake .. -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### FreeBSD
```bash
cd platforms/build-bsd
cmake .. -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Running

```bash
./pzep-gui file.txt
```

---

## Keybindings

See main pZep documentation for full Vim keybinding reference.