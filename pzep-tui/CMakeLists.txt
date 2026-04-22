cmake_minimum_required(VERSION 3.15)
project(pzep-tui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE C:/Users/casse/github/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET x64-windows)
set(unofficial-pdcurses_DIR C:/Users/casse/github/vcpkg/installed/x64-windows/share/unofficial-pdcurses)

find_package(unofficial-pdcurses CONFIG REQUIRED)

set(ROOT_DIR C:/Users/casse/github/nzep)

include_directories(
    ${ROOT_DIR}/include
    ${ROOT_DIR}/include/zep
    ${ROOT_DIR}/include/zep/mcommon
    ${ROOT_DIR}/include/zep/ansi
    ${ROOT_DIR}/src
)

set(ZEP_SOURCES
    ${ROOT_DIR}/src/buffer.cpp
    ${ROOT_DIR}/src/range_markers.cpp
    ${ROOT_DIR}/src/glyph_iterator.cpp
    ${ROOT_DIR}/src/commands.cpp
    ${ROOT_DIR}/src/display.cpp
    ${ROOT_DIR}/src/editor.cpp
    ${ROOT_DIR}/src/filesystem.cpp
    ${ROOT_DIR}/src/git.cpp
    ${ROOT_DIR}/src/fold.cpp
    ${ROOT_DIR}/src/indexer.cpp
    ${ROOT_DIR}/src/keymap.cpp
    ${ROOT_DIR}/src/line_widgets.cpp
    ${ROOT_DIR}/src/mcommon/animation/timer.cpp
    ${ROOT_DIR}/src/mcommon/string/stringutils.cpp
    ${ROOT_DIR}/src/mcommon/file/path.cpp
    ${ROOT_DIR}/src/mode.cpp
    ${ROOT_DIR}/src/mode_search.cpp
    ${ROOT_DIR}/src/mode_standard.cpp
    ${ROOT_DIR}/src/mode_tree.cpp
    ${ROOT_DIR}/src/mode_vim.cpp
    ${ROOT_DIR}/src/mode_repl.cpp
    ${ROOT_DIR}/src/regress.cpp
    ${ROOT_DIR}/src/scroller.cpp
    ${ROOT_DIR}/src/splits.cpp
    ${ROOT_DIR}/src/syntax.cpp
    ${ROOT_DIR}/src/syntax_providers.cpp
    ${ROOT_DIR}/src/syntax_rainbow_brackets.cpp
    ${ROOT_DIR}/src/syntax_tree.cpp
    ${ROOT_DIR}/src/syntax_markdown.cpp
    ${ROOT_DIR}/src/tab_window.cpp
    ${ROOT_DIR}/src/theme.cpp
    ${ROOT_DIR}/src/window.cpp
    ${ROOT_DIR}/src/commands_terminal_stub.cpp
    ${ROOT_DIR}/src/console/display_console.cpp
    ${ROOT_DIR}/src/ansi/display_ansi.cpp
)

add_library(Zep STATIC ${ZEP_SOURCES})

target_include_directories(Zep PRIVATE
    ${ROOT_DIR}/src
    ${ROOT_DIR}/src/mcommon
    ${ROOT_DIR}/include
    C:/Users/casse/github/vcpkg/installed/x64-windows/include
)

target_compile_definitions(Zep PRIVATE ZEP_FEATURE_CPP_FILE_SYSTEM)

target_link_libraries(Zep PRIVATE
    C:/Users/casse/github/vcpkg/installed/x64-windows/lib/pdcurses.lib
)

add_executable(pzep-tui main.cpp)

target_link_libraries(pzep-tui PRIVATE
    Zep
    C:/Users/casse/github/vcpkg/installed/x64-windows/lib/pdcurses.lib
)

target_include_directories(pzep-tui PRIVATE
    C:/Users/casse/github/vcpkg/installed/x64-windows/include
)
