# pzep-tui: Terminal-Based Vim-Like Text Editor

A lightweight, terminal-based text editor built on the Zep editing engine with Vim keybindings.

## Features

- **Vim-style editing** - Normal, Insert, Visual, and Ex modes
- **Full editor UI** - Tab bar, statusline (airline), and command region
- **ANSI terminal rendering** - Works in any ANSI-compatible terminal
- **Cross-platform** - Windows, Linux, macOS support via PDCurses

## Architecture

```
┌─────────────────────────────────────────────┐
│ Tab Region (if multiple tabs)                │
├─────────────────────────────────────────────┤
│                                             │
│ Tab Content Region (split windows)            │
│ ┌─────────────────────────────────────┐    │
│ │ Airline/Statusbar                     │    │
│ │ [MODE] [file.txt] [1:5]            │    │
│ └─────────────────────────────────────┘    │
│                                             │
├─────────────────────────────────────────────┤
│ Command Region (:commands, search, etc.)       │
└─────────────────────────────────────────────┘
```

### Editor Components

| Component | Description |
|-----------|-------------|
| **Tab Region** | Top bar showing open buffers/tabs |
| **Tab Content** | Main editing area with text windows |
| **Airline** | Statusbar showing mode, file, cursor position |
| **Command Region** | Ex commands, search prompts, output |

### Display Backend

- `ZepDisplay_ANSI` - Renders to ANSI escape sequences
- `ZepFont_ANSI` - Monospace font metrics (configurable char size)

## Keybindings

### Normal Mode

| Key | Action |
|-----|--------|
| `h/j/k/l` | Left/Down/Up/Right |
| `i` | Enter Insert mode |
| `v` | Enter Visual mode |
| `V` | Visual line mode |
| `ESC` | Return to Normal mode |
| `0/$` | Line begin/end |
| `w/b` | Word forward/backward |
| `gg/G` | File begin/end |
| `u` | Undo |
| `Ctrl-r` | Redo |
| `x` | Delete char |
| `dd` | Delete line |
| `yy` | Yank line |
| `p/P` | Paste |
| `/` | Search forward |
| `n/N` | Next/Previous search |
| `:` | Ex command |

### Insert Mode

| Key | Action |
|-----|--------|
| `ESC` | Return to Normal mode |
| Arrow keys | Navigate |
| `Backspace` | Delete |
| `Enter` | New line |

### Visual Mode

| Key | Action |
|-----|--------|
| `y` | Yank selection |
| `d` | Delete selection |
| `c` | Change selection |
| `ESC` | Exit Visual mode |

## Ex Commands

| Command | Description |
|---------|------------|
| `:w` | Save file |
| `:q` | Quit |
| `:q!` | Quit without saving |
| `:wq` | Save and quit |
| `:e <file>` | Open file |
| `:bn` | Next buffer |
| `:bp` | Previous buffer |
| `:sp <file>` | Split horizontally |

## Build Instructions

### Prerequisites

- C++17 compiler (MSVC 2022, GCC, Clang)
- CMake 3.15+
- vcpkg (Windows)

### Windows (vcpkg)

```powershell
# Install PDCurses
.\vcpkg install unofficial-pdcurses:x64-windows

# Build
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Linux

```bash
# Debian/Ubuntu
sudo apt install libpdcurses-dev

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### macOS

```bash
brew install pdcurses

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Installation

### Per-User Installation (Corporate/PIM)

For environments requiring user-level installation (no admin required):

```powershell
# Create user directory
mkdir $LOCALAPPDATA\pzep-tui
copy Release\pzep-tui.exe $LOCALAPPDATA\pzep-tui\

# Add to PATH (user level)
setx PATH "$PATH;$LOCALAPPDATA\pzep-tui"
```

Default user directories:
- **Windows**: `%LOCALAPPDATA%\pzep-tui`
- **Linux**: `~/.local/pzep-tui`
- **macOS**: `~/Library/Application Support/pzep-tui`

### All-Users Installation

For system-wide installation (requires admin):

```powershell
# Windows (per-machine)
copy Release\pzep-tui.exe "C:\Program Files\pzep-tui\"
copy -r . "C:\Program Files\pzep-tui\"

# Add to system PATH
setx /M PATH "$PATH;C:\Program Files\pzep-tui"
```

Default install directories:
- **Windows**: `C:\Program Files\pzep-tui`
- **Linux**: `/usr/local/pzep-tui`
- **macOS**: `/Applications/pzep-tui`

### Package Managers

#### Windows Package Managers (Planned)

```powershell
# Winget (when published)
winget install pzep-tui

# Chocolatey (when published)
choco install pzep-tui
```

#### Portable Install

```powershell
# Extract and run
Expand-Archive pzep-tui.zip -DestinationPath .
.\pzep-tui.exe
```

## Deployment

### Windows Installer Options

1. **NSIS Installer**
   - Per-user or all-users installation
   - Start menu shortcuts
   - Uninstaller

2. **MSI Installer**
   - Group Policy support
   - Enterprise deployment

3. **Portable ZIP**
   - No installation required
   - Extract and run anywhere

### CI/CD Pipeline (GitHub Actions)

```yaml
# Release workflow
- name: Build Windows
  run: |
    mkdir build && cd build
    cmake .. -G "Visual Studio 17 2022" -A x64
    cmake --build . --config Release

- name: Create Installer
  run: |
    # Package with NSIS or WiX
    makensis installer.nsi
```

### GitHub Releases

Artifacts hosted at: `https://github.com/<owner>/nzep/releases`

Assets:
- `pzep-tui-x64-windows-installer.exe`
- `pzep-tui-x64-windows-portable.zip`
- `pzep-tui-linux-x64.tar.gz`
- `pzep-tui-macos-universal.tar.gz`

## Configuration

Configuration files (JSON):

| Location | Purpose |
|----------|---------|
| `~/.zep/zep.cfg` | Global settings |
| `~/.zep/theme.json` | Color themes |

### Sample Configuration

```json
{
    "theme": "dark",
    "tabSize": 4,
    "autoSave": true,
    "fontSize": 16
}
```

## Dependencies

| Dependency | Version | License | Purpose |
|-----------|---------|---------|---------|
| Zep | main | MIT | Editor engine |
| PDCurses | latest | NCURSES | Terminal I/O |
| vcpkg | latest | MIT | Package manager |

## License

Same as Zep - MIT License

## See Also

- [Zep Editor](https://github.com/djwisdom/nzep) - Main project
- [Zep Documentation](https://github.com/djwisdom/nzep/docs)
- [Vim Reference](http://www.vim.org/docs.php)