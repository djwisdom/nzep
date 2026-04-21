# pzep-tui
## A Terminal-Based Vim-Like Editor

> **Status: Coming Soon**
> 
> The TUI version is not yet implemented. For now, use pzep-gui (Raylib) for a graphical experience, or the main nZep editor.

---

## Planned Features

- Full Vim emulation (motion, operators, ex commands)
- Terminal renderer using ANSI escape codes
- Multiple file editing with tabs/splits
- Syntax highlighting
- Git integration
- Macros and folds
- True terminal/TTY experience (not graphical window)

---

## For Now: Use pzep-gui

If you want to try pZep today, use the GUI version:

```bash
# Build pzep-gui
cd pzep-gui
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run
./pzep-gui file.txt
```

See [pzep-gui README](../pzep-gui) for full build instructions.

---

## Documentation

See [User Guide](../docs/USER_GUIDE.md) for Vim keybinding reference.