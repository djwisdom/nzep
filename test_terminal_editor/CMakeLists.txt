cmake_minimum_required(VERSION 3.15)
project(pzep-tui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE C:/Users/casse/github/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET x64-windows)

find_package(unofficial-pdcurses CONFIG REQUIRED)

include_directories(
    C:/Users/casse/github/nzep/include
    C:/Users/casse/github/nzep/include/zep
    C:/Users/casse/github/nzep/include/zep/mcommon
    C:/Users/casse/github/nzep/include/zep/ansi
    C:/Users/casse/github/nzep/src
)

set(ZEP_SOURCES
    C:/Users/casse/github/nzep/src/buffer.cpp
    C:/Users/casse/github/nzep/src/range_markers.cpp
    C:/Users/casse/github/nzep/src/glyph_iterator.cpp
    C:/Users/casse/github/nzep/src/commands.cpp
    C:/Users/casse/github/nzep/src/display.cpp
    C:/Users/casse/github/nzep/src/editor.cpp
    C:/Users/casse/github/nzep/src/filesystem.cpp
    C:/Users/casse/github/nzep/src/git.cpp
    C:/Users/casse/github/nzep/src/fold.cpp
    C:/Users/casse/github/nzep/src/indexer.cpp
    C:/Users/casse/github/nzep/src/keymap.cpp
    C:/Users/casse/github/nzep/src/line_widgets.cpp
    C:/Users/casse/github/nzep/src/mcommon/animation/timer.cpp
    C:/Users/casse/github/nzep/src/mcommon/string/stringutils.cpp
    C:/Users/casse/github/nzep/src/mcommon/file/path.cpp
    C:/Users/casse/github/nzep/src/mode.cpp
    C:/Users/casse/github/nzep/src/mode_search.cpp
    C:/Users/casse/github/nzep/src/mode_standard.cpp
    C:/Users/casse/github/nzep/src/mode_tree.cpp
    C:/Users/casse/github/nzep/src/mode_vim.cpp
    C:/Users/casse/github/nzep/src/mode_repl.cpp
    C:/Users/casse/github/nzep/src/regress.cpp
    C:/Users/casse/github/nzep/src/scroller.cpp
    C:/Users/casse/github/nzep/src/splits.cpp
    C:/Users/casse/github/nzep/src/syntax.cpp
    C:/Users/casse/github/nzep/src/syntax_providers.cpp
    C:/Users/casse/github/nzep/src/syntax_rainbow_brackets.cpp
    C:/Users/casse/github/nzep/src/syntax_tree.cpp
    C:/Users/casse/github/nzep/src/syntax_markdown.cpp
    C:/Users/casse/github/nzep/src/tab_window.cpp
    C:/Users/casse/github/nzep/src/theme.cpp
    C:/Users/casse/github/nzep/src/window.cpp
    C:/Users/casse/github/nzep/src/commands_terminal_stub.cpp
    C:/Users/casse/github/nzep/src/console/display_console.cpp
    C:/Users/casse/github/nzep/src/ansi/display_ansi.cpp
)

add_library(Zep STATIC ${ZEP_SOURCES})

target_include_directories(Zep PRIVATE
    C:/Users/casse/github/nzep/src
    C:/Users/casse/github/nzep/src/mcommon
    C:/Users/casse/github/nzep/include
    C:/Users/casse/github/vcpkg/installed/x64-windows/include
)

target_compile_definitions(Zep PRIVATE ZEP_FEATURE_CPP_FILE_SYSTEM)

target_link_libraries(Zep PRIVATE
    C:/Users/casse/github/vcpkg/installed/x64-windows/lib/pdcurses.lib
)

add_executable(pzep-tui ../test_terminal_editor/main.cpp)

target_link_libraries(pzep-tui PRIVATE
    Zep
    C:/Users/casse/github/vcpkg/installed/x64-windows/lib/pdcurses.lib
)

target_include_directories(pzep-tui PRIVATE
    C:/Users/casse/github/vcpkg/installed/x64-windows/include
)