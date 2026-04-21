# nZep Changelog
## What's New in nZep (Notification Editor)

---

## v0.2.0 - High Impact Features Update

### 1. Find/Replace (:s command) - NEW

**Location:** `src/mode_vim.cpp`

The substitute command now works properly:

| Command | Description |
|---------|-------------|
| `:s/foo/bar/` | Replace first match on current line |
| `:s/foo/bar/g` | Replace all matches on current line |
| `:%s/foo/bar/g` | Replace all matches in entire file |
| `:Ns/foo/bar/g` | Replace on N lines |

**Supported flags:**
- `g` - global (replace all on line)
- `i` - ignore case
- `c` - confirm (shows message)

**Example:**
```vim
:s/old/new/       " Replace first 'old' with 'new'
:s/old/new/g      " Replace all 'old' with 'new'  
:%s/foo/bar/g     " Replace all 'foo' with 'bar' in file
```

---

### 2. Auto-Indent - NEW

**Location:** `src/mode.cpp`, `buffer.cpp`

Enable with:
```vim
:set autoindent    " Enable
:set noautoindent " Disable
```

**Features:**
- **Basic**: Press Enter, get previous line's indent
- **C-style**: `{` indents, `}` de-indents
- **Tab**: Increase indentation
- **Shift+Tab**: Decrease indentation

**Usage:**
```python
# With autoindent enabled:
def foo():
    # Press Enter here - gets 4 spaces indent
    pass
```

---

### 3. Ex Commands - NEW

**Location:** `src/mode_vim.cpp`

Essential Vim commands implemented:

| Command | Description |
|---------|-------------|
| `:q` | Quit (fails if unsaved) |
| `:q!` | Quit without saving |
| `:qa` | Quit all buffers |
| `:w` | Write/save |
| `:w file.txt` | Save as |
| `:wq` | Write and quit |
| `:n` | Next buffer |
| `:ls` / `:buffers` | List buffers |
| `:g/pattern/command` | Global: run on matching lines |

**Examples:**
```vim
:w              " Save current file
:w newfile.txt  " Save as new file
:q!             " Quit without saving
:g/^$/d         " Delete empty lines
:g/TODO/p       " Print all TODO lines
```

---

### 4. UTF-8 Complete Support - FIXED

**Location:** `src/buffer.cpp`, `src/mode_search.cpp`, `stringutils.h`

**Fixed:**
- Replace function now handles multi-byte characters
- Search input properly encodes Unicode
- Added utility functions:
  - `utf8_strlen()` - count codepoints
  - `utf8_next_codepoint()` - extract next codepoint
  - `utf8_substr()` - UTF-8 aware substring

**Before (broken):**
```
"ü" was treated as 2 characters
```

**After (fixed):**
```
"ü" is properly 1 character
```

---

## v0.1.0 - Original Zep + nZep

### Editor Features
- Modal (Vim) + modeless (Standard) editing
- Tabs and splits
- Syntax highlighting (C++, Python, Rust, Go, etc.)
- Theme support (light/dark)
- REPL integration
- ImGui and Qt renderers
- Cross-platform (Windows, Linux, FreeBSD)

### Notification Features
- 12 notification types
- ImGui and Terminal renderers
- 19 unit tests
- No external dependencies

---

## Upgrading from Original Zep

All original features still work. New features are additive:

| Old Behavior | New Behavior |
|--------------|--------------|
| No :s command | `:s/foo/bar/g` works |
| No auto-indent | `:set autoindent` works |
| No :q/:g | All ex commands work |
| UTF-8 buggy | UTF-8 fully works |

---

## Breaking Changes

None. All new features are opt-in.

---

## Test Status

Run tests:
```bash
./build/tests/unittests.exe --gtest_filter="*"
```

Expected: All tests pass including new features.

---

*For full documentation, see docs/NOTIFICATIONS_USER_MANUAL.md*