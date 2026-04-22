# pzep-tui: Terminal-Based Vim-Like Text Editor

**NOTE**: The actual source code has moved to `pzep-tui/` directory. This folder contains planning notes only.

## Status

- [x] Source migrated from `test_terminal_editor/`
- [x] PDCurses backend working
- [ ] FTXUI backend (planned, but PDCurses works better)
- [ ] Proper terminal font detection
- [ ] Complete keybinding support

## Current Implementation

The active implementation is at `../pzep-tui/`:

```
pzep-tui/
├── CMakeLists.txt    # Build config
├── main.cpp         # Terminal editor entry point
├── README.md        # Full documentation
└── build/          # Visual Studio build output
```

## Building

```powershell
cd pzep-tui/build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Known Issues

1. Font metrics - needs actual terminal font detection
2. Some Vim keybindings not working
3. No mouse support

## TODO

- [ ] Implement proper font detection on each platform
- [ ] Add mouse support (clicks, drags)
- [ ] Add clipboard integration
- [ ] Test on Linux/macOS