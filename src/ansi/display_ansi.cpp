#include "zep/ansi/display_ansi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Zep
{

#ifdef _WIN32
static bool EnableVirtualTerminalProcessing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return false;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return false;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return SetConsoleMode(hOut, dwMode) != 0;
}

static void GetConsoleSize(int& rows, int& cols)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        rows = 24;
        cols = 80;
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else
    {
        rows = 24;
        cols = 80;
    }
}
#endif

ZepDisplay_ANSI::ZepDisplay_ANSI()
    : ZepDisplay()
{
#ifdef _WIN32
    EnableVirtualTerminalProcessing();
    GetConsoleSize(m_terminalHeight, m_terminalWidth);
#else
    const char* lines = getenv("LINES");
    const char* cols = getenv("COLS");
    m_terminalHeight = (lines) ? std::atoi(lines) : 24;
    m_terminalWidth = (cols) ? std::atoi(cols) : 80;

    if (m_terminalHeight <= 0)
        m_terminalHeight = 24;
    if (m_terminalWidth <= 0)
        m_terminalWidth = 80;
#endif

    m_spDefaultFont = std::make_shared<ZepFontNull>(*this);
    m_spDefaultFont->SetPixelHeight(16);
    m_ansiEnabled = true;

    ClearScreen();
}

ZepDisplay_ANSI::~ZepDisplay_ANSI()
{
    std::cout << "\x1b[0m";
    ClearScreen();
    std::cout << std::flush;
}

void ZepDisplay_ANSI::DrawLine(const NVec2f& start, const NVec2f& end, const NVec4f& color, float width) const
{
    (void)start;
    (void)end;
    (void)color;
    (void)width;
}

void ZepDisplay_ANSI::DrawChars(ZepFont& font, const NVec2f& pos, const NVec4f& col,
    const uint8_t* text_begin, const uint8_t* text_end) const
{
    if (!text_begin || !*text_begin)
        return;

    if (text_end == nullptr)
        text_end = text_begin + strlen((const char*)text_begin);

    auto charSize = font.GetDefaultCharSize();
    int charWidth = (int)charSize.x;
    int charHeight = (int)charSize.y;

    int colPos = (int)(pos.x / charWidth);
    int rowPos = (int)(pos.y / charHeight);

    MoveCursor(rowPos + 1, colPos + 1);

    int r = (int)(col.x * 255);
    int g = (int)(col.y * 255);
    int b = (int)(col.z * 255);
    std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";

    std::string text(reinterpret_cast<const char*>(text_begin), text_end - text_begin);
    std::cout << text;

    std::cout << "\x1b[0m";
}

void ZepDisplay_ANSI::DrawRectFilled(const NRectf& rc, const NVec4f& col) const
{
    if (m_ansiEnabled)
    {
        auto charSize = m_spDefaultFont->GetDefaultCharSize();
        int charWidth = (int)charSize.x;
        int charHeight = (int)charSize.y;

        int left = (int)(rc.topLeftPx.x / charWidth);
        int top = (int)(rc.topLeftPx.y / charHeight);
        int right = (int)(rc.bottomRightPx.x / charWidth);
        int bottom = (int)(rc.bottomRightPx.y / charHeight);

        int r = (int)(col.x * 255);
        int g = (int)(col.y * 255);
        int b = (int)(col.z * 255);
        int bgR = (int)(col.w * 255);
        int bgG = (int)(col.w * 255);
        int bgB = (int)(col.w * 255);

        MoveCursor(top + 1, left + 1);
        std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
        std::cout << "\x1b[48;2;" << bgR << ";" << bgG << ";" << bgB << "m";

        for (int y = top; y <= bottom; y++)
        {
            MoveCursor(y + 1, left + 1);
            int width = right - left + 1;
            for (int x = 0; x < width; x++)
                std::cout << " ";
        }

        std::cout << "\x1b[0m";
    }
}

void ZepDisplay_ANSI::SetClipRect(const NRectf& rc)
{
    m_clipRect = rc;
    m_clipEnabled = true;
}

ZepFont& ZepDisplay_ANSI::GetFont(ZepTextType type)
{
    if (m_fonts[(int)type] == nullptr)
    {
        return *m_spDefaultFont;
    }
    return *m_fonts[(int)type];
}

void ZepDisplay_ANSI::BeginFrame()
{
    if (m_ansiEnabled)
    {
        std::cout << "\x1b[s";
    }
}

void ZepDisplay_ANSI::EndFrame()
{
    if (m_ansiEnabled)
    {
        std::cout << "\x1b[0m";
        std::cout << "\x1b[u";
        std::cout << std::flush;
    }
}

bool ZepDisplay_ANSI::ShouldClose() const
{
    return !m_running;
}

int ZepDisplay_ANSI::GetKeyPressed()
{
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD irInBuf[1];
    DWORD cNumRead;

    if (!PeekConsoleInput(hIn, irInBuf, 1, &cNumRead))
        return 0;

    if (cNumRead == 0)
        return 0;

    ReadConsoleInput(hIn, irInBuf, 1, &cNumRead);

    if (irInBuf[0].EventType == KEY_EVENT && irInBuf[0].Event.KeyEvent.bKeyDown)
    {
        WORD vk = irInBuf[0].Event.KeyEvent.wVirtualKeyCode;
        if (vk == 'Q' || vk == 'q' || vk == VK_ESCAPE)
            m_running = false;
        return irInBuf[0].Event.KeyEvent.uChar.AsciiChar;
    }
    return 0;
#else
    struct termios old, newt;
    tcgetattr(STDIN_FILENO, &old);
    newt = old;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    if (ch == 'q' || ch == 'Q' || ch == 27)
        m_running = false;

    return ch;
#endif
}

int ZepDisplay_ANSI::GetCharPressed()
{
    return GetKeyPressed();
}

bool ZepDisplay_ANSI::IsKeyDown(int key)
{
    return GetKeyPressed() == key;
}

void ZepDisplay_ANSI::MoveCursor(int row, int col) const
{
    if (m_ansiEnabled)
    {
        std::cout << "\x1b[" << row << ";" << col << "H";
    }
}

void ZepDisplay_ANSI::SaveCursor()
{
    if (m_ansiEnabled)
        std::cout << "\x1b[s";
}

void ZepDisplay_ANSI::RestoreCursor()
{
    if (m_ansiEnabled)
        std::cout << "\x1b[u";
}

void ZepDisplay_ANSI::ClearScreen()
{
    if (m_ansiEnabled)
        std::cout << "\x1b[2J\x1b[H";
}

} // namespace Zep