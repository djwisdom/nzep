#pragma once

#include "zep/display.h"

#include <memory>
#include <string>
#include <vector>

namespace Zep
{

class ZepDisplay_ANSI : public ZepDisplay
{
public:
    ZepDisplay_ANSI();
    virtual ~ZepDisplay_ANSI();

    virtual void DrawLine(const NVec2f& start, const NVec2f& end, const NVec4f& color = NVec4f(1.0f), float width = 1.0f) const override;
    virtual void DrawChars(ZepFont& font, const NVec2f& pos, const NVec4f& col, const uint8_t* text_begin, const uint8_t* text_end = nullptr) const override;
    virtual void DrawRectFilled(const NRectf& rc, const NVec4f& col = NVec4f(1.0f)) const override;
    virtual void SetClipRect(const NRectf& rc) override;
    virtual ZepFont& GetFont(ZepTextType type) override;

    void BeginFrame();
    void EndFrame();
    bool ShouldClose() const;

    int GetKeyPressed();
    int GetCharPressed();
    bool IsKeyDown(int key);

    int GetTerminalWidth() const
    {
        return m_terminalWidth;
    }
    int GetTerminalHeight() const
    {
        return m_terminalHeight;
    }

private:
    void EnableANSI();
    void DisableEcho();
    void EnableEcho();
    void SaveTerminalState();
    void RestoreTerminalState();
    void ClearScreen();
    void MoveCursor(int row, int col) const;
    void SetForeColor(int r, int g, int b);
    void SetBackColor(int r, int g, int b);
    void ResetColors();
    void SaveCursor();
    void RestoreCursor();
    void ShowCursor();
    void HideCursor();
    int RGBToAnsi(int r, int g, int b);

    int m_terminalWidth = 80;
    int m_terminalHeight = 24;

    std::shared_ptr<ZepFont> m_spDefaultFont;
    NRectf m_clipRect;
    bool m_clipEnabled = false;
    bool m_running = true;
    bool m_ansiEnabled = false;
};

} // namespace Zep