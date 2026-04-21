// pzep-gui: A Vim-like editor using Raylib
// Main application entry point

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "zep/buffer.h"
#include "zep/editor.h"
#include "zep/mode_standard.h"
#include "zep/mode_vim.h"
#include "zep/raylib/display_raylib.h"
#include "zep/tab_window.h"
#include "zep/theme.h"
#include "zep/window.h"

// Key mapping from Raylib to Zep
#include "zep/mcommon/keymap.h"

using namespace Zep;

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 800;

void PrintWelcome()
{
    std::cout << "=== pZep-GUI v0.1.0 ===" << std::endl;
    std::cout << "A Vim-like editor powered by Raylib" << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  ESC       - Return to normal mode" << std::endl;
    std::cout << "  i         - Enter insert mode" << std::endl;
    std::cout << "  :         - Enter ex command" << std::endl;
    std::cout << "  h/j/k/l   - Navigation (normal mode)" << std::endl;
    std::cout << std::endl;
    std::cout << "Ex commands:" << std::endl;
    std::cout << "  :w        - Save file" << std::endl;
    std::cout << "  :q        - Quit" << std::endl;
    std::cout << "  :e <file> - Open file" << std::endl;
    std::cout << "  :bn       - Next buffer" << std::endl;
    std::cout << "  :bp       - Previous buffer" << std::endl;
    std::cout << "  :set      - Toggle options" << std::endl;
    std::cout << std::endl;
}

// Map Raylib key codes to Zep key codes
static long MapKey(int raylibKey)
{
    switch (raylibKey)
    {
    case KEY_NULL:
        return 0;
    case KEY_APOSTROPHE:
        return '\'';
    case KEY_COMMA:
        return ',';
    case KEY_MINUS:
        return '-';
    case KEY_PERIOD:
        return '.';
    case KEY_SLASH:
        return '/';
    case KEY_ZERO:
        return '0';
    case KEY_ONE:
        return '1';
    case KEY_TWO:
        return '2';
    case KEY_THREE:
        return '3';
    case KEY_FOUR:
        return '4';
    case KEY_FIVE:
        return '5';
    case KEY_SIX:
        return '6';
    case KEY_SEVEN:
        return '7';
    case KEY_EIGHT:
        return '8';
    case KEY_NINE:
        return '9';
    case KEY_SEMICOLON:
        return ';';
    case KEY_EQUAL:
        return '=';
    case KEY_A:
        return 'a';
    case KEY_B:
        return 'b';
    case KEY_C:
        return 'c';
    case KEY_D:
        return 'd';
    case KEY_E:
        return 'e';
    case KEY_F:
        return 'f';
    case KEY_G:
        return 'g';
    case KEY_H:
        return 'h';
    case KEY_I:
        return 'i';
    case KEY_J:
        return 'j';
    case KEY_K:
        return 'k';
    case KEY_L:
        return 'l';
    case KEY_M:
        return 'm';
    case KEY_N:
        return 'n';
    case KEY_O:
        return 'o';
    case KEY_P:
        return 'p';
    case KEY_Q:
        return 'q';
    case KEY_R:
        return 'r';
    case KEY_S:
        return 's';
    case KEY_T:
        return 't';
    case KEY_U:
        return 'u';
    case KEY_V:
        return 'v';
    case KEY_W:
        return 'w';
    case KEY_X:
        return 'x';
    case KEY_Y:
        return 'y';
    case KEY_Z:
        return 'z';
    case KEY_LEFT_BRACKET:
        return '[';
    case KEY_BACKSLASH:
        return '\\';
    case KEY_RIGHT_BRACKET:
        return ']';
    case KEY_GRAVE:
        return '`';
    case KEY_SPACE:
        return ' ';
    case KEY_ESCAPE:
        return ExtKeys::ESCAPE;
    case KEY_ENTER:
        return ExtKeys::RETURN;
    case KEY_TAB:
        return ExtKeys::TAB;
    case KEY_BACKSPACE:
        return ExtKeys::BACKSPACE;
    case KEY_UP:
        return ExtKeys::UP;
    case KEY_DOWN:
        return ExtKeys::DOWN;
    case KEY_LEFT:
        return ExtKeys::LEFT;
    case KEY_RIGHT:
        return ExtKeys::RIGHT;
    case KEY_F1:
        return ExtKeys::F1;
    case KEY_F2:
        return ExtKeys::F2;
    case KEY_F3:
        return ExtKeys::F3;
    case KEY_F4:
        return ExtKeys::F4;
    case KEY_F5:
        return ExtKeys::F5;
    case KEY_F6:
        return ExtKeys::F6;
    case KEY_F7:
        return ExtKeys::F7;
    case KEY_F8:
        return ExtKeys::F8;
    case KEY_F9:
        return ExtKeys::F9;
    case KEY_F10:
        return ExtKeys::F10;
    case KEY_F11:
        return ExtKeys::F11;
    case KEY_F12:
        return ExtKeys::F12;
    case KEY_HOME:
        return ExtKeys::HOME;
    case KEY_END:
        return ExtKeys::END;
    case KEY_PAGE_UP:
        return ExtKeys::PAGE_UP;
    case KEY_PAGE_DOWN:
        return ExtKeys::PAGE_DOWN;
    case KEY_INSERT:
        return ExtKeys::INSERT;
    case KEY_DELETE:
        return ExtKeys::DEL;
    case KEY_KP_0:
        return '0';
    case KEY_KP_1:
        return '1';
    case KEY_KP_2:
        return '2';
    case KEY_KP_3:
        return '3';
    case KEY_KP_4:
        return '4';
    case KEY_KP_5:
        return '5';
    case KEY_KP_6:
        return '6';
    case KEY_KP_7:
        return '7';
    case KEY_KP_8:
        return '8';
    case KEY_KP_9:
        return '9';
    case KEY_KP_ENTER:
        return ExtKeys::RETURN;
    case KEY_KP_ADD:
        return '+';
    case KEY_KP_SUBTRACT:
        return '-';
    case KEY_KP_MULTIPLY:
        return '*';
    case KEY_KP_DIVIDE:
        return '/';
    default:
        return 0;
    }
}

int main(int argc, char* argv[])
{
    PrintWelcome();

    // Create Raylib display
    ZepDisplay_Raylib display(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create the Zep editor
    ZepEditor editor(&display, ".");

    // Register Vim mode
    editor.RegisterMode(std::make_shared<ZepMode_Vim>(editor));

    // Default to Vim mode
    editor.SetCurrentMode("Vim");

    // Load file if provided
    std::string filename = (argc > 1) ? argv[1] : "";
    if (!filename.empty())
    {
        editor.GetFileBuffer(filename);
    }
    else
    {
        editor.GetEmptyBuffer("untitled");
    }

    std::cout << "Editor initialized. Window should appear." << std::endl;
    std::cout << "Press ESC then q to quit, or :q in the editor." << std::endl;

    // Main loop
    while (!display.ShouldClose())
    {
        display.BeginFrame();

        // Handle input
        // Check for character input (text)
        int codepoint = display.GetCharPressed();
        while (codepoint != 0)
        {
            if (codepoint >= 32 && codepoint <= 126)
            {
                // ASCII printable
                editor.AddKeyPress(codepoint, 0);
            }
            else if (codepoint > 127)
            {
                // UTF-8 - pass as-is for now
                editor.AddKeyPress(codepoint, 0);
            }
            codepoint = display.GetCharPressed();
        }

        // Check for special key input
        int key = display.GetKeyPressed();
        while (key != 0)
        {
            long zepKey = MapKey(key);
            if (zepKey != 0)
            {
                editor.AddKeyPress(zepKey, 0);
            }
            key = display.GetKeyPressed();
        }

        // Render the editor
        auto pWindow = editor.GetActiveWindow();
        if (pWindow)
        {
            pWindow->Display();
        }

        display.EndFrame();
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}