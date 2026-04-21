#include "zep/raylib/display_raylib.h"
#include <cstring>

namespace Zep
{

//------------------------------------------------------------------------------------------
// ZepFont_Raylib
//------------------------------------------------------------------------------------------

ZepFont_Raylib::ZepFont_Raylib(ZepDisplay& display, ::Font font, int pixelHeight)
    : ZepFont(display)
    , m_font(font)
    , m_baseSize(pixelHeight)
{
    m_pixelHeight = pixelHeight;
}

ZepFont_Raylib::~ZepFont_Raylib()
{
}

void ZepFont_Raylib::SetPixelHeight(int height)
{
    m_pixelHeight = height;
    m_charCacheDirty = true;
}

NVec2f ZepFont_Raylib::GetTextSize(const uint8_t* pBegin, const uint8_t* pEnd) const
{
    if (pBegin == nullptr || pBegin == pEnd)
        return NVec2f(0, (float)m_pixelHeight);

    if (pEnd == nullptr)
        pEnd = pBegin + strlen((const char*)pBegin);

    float width = MeasureTextEx(m_font, (const char*)pBegin, (float)m_pixelHeight, 1.0f).x;
    return NVec2f(width, (float)m_pixelHeight);
}

//------------------------------------------------------------------------------------------
// ZepDisplay_Raylib
//------------------------------------------------------------------------------------------

ZepDisplay_Raylib::ZepDisplay_Raylib(int width, int height)
    : ZepDisplay()
    , m_width(width)
    , m_height(height)
    , m_clipEnabled(false)
{
    InitWindow(width, height, "pZep-GUI - Vim-like Editor");
    SetTargetFPS(60);

    // Load default font with UTF-8 support
    // Using built-in default font
    m_defaultFont = GetFontDefault();
    if (m_defaultFont.texture.id == 0)
    {
        // Try loading a basic font if default fails
        // In production, you'd bundle a TTF file
    }

    // Create default font entry
    m_spDefaultFont = std::make_shared<ZepFont_Raylib>(*this, m_defaultFont, 16);
}

ZepDisplay_Raylib::~ZepDisplay_Raylib()
{
    if (m_defaultFont.texture.id != 0 && m_defaultFont.texture.id != GetFontDefault().texture.id)
    {
        UnloadFont(m_defaultFont);
    }
    CloseWindow();
}

void ZepDisplay_Raylib::DrawLine(const NVec2f& start, const NVec2f& end, const NVec4f& color, float width) const
{
    Color c = {
        (unsigned char)(color.x * 255.0f),
        (unsigned char)(color.y * 255.0f),
        (unsigned char)(color.z * 255.0f),
        (unsigned char)(color.w * 255.0f)
    };
    DrawLineEx({ start.x, start.y }, { end.x, end.y }, width, c);
}

void ZepDisplay_Raylib::DrawChars(ZepFont& font, const NVec2f& pos, const NVec4f& col,
    const uint8_t* text_begin, const uint8_t* text_end) const
{
    if (!text_begin || !*text_begin)
        return;

    if (text_end == nullptr)
        text_end = text_begin + strlen((const char*)text_begin);

    Color c = {
        (unsigned char)(col.x * 255.0f),
        (unsigned char)(col.y * 255.0f),
        (unsigned char)(col.z * 255.0f),
        (unsigned char)(col.w * 255.0f)
    };

    // Get font size from ZepFont
    int fontSize = font.GetPixelHeight();
    if (fontSize <= 0)
        fontSize = 16;

    // Cast to non-const for Raylib
    DrawTextEx(m_defaultFont, (const char*)text_begin, { pos.x, pos.y }, (float)fontSize, 1.0f, c);
}

void ZepDisplay_Raylib::DrawRectFilled(const NRectf& rc, const NVec4f& col) const
{
    Color c = {
        (unsigned char)(col.x * 255.0f),
        (unsigned char)(col.y * 255.0f),
        (unsigned char)(col.z * 255.0f),
        (unsigned char)(col.w * 255.0f)
    };
    DrawRectangle((int)rc.x, (int)rc.y, (int)rc.w, (int)rc.h, c);
}

void ZepDisplay_Raylib::SetClipRect(const NRectf& rc)
{
    m_clipRect = rc;
    m_clipEnabled = true;
    // Raylib uses BeginScissorMode for clipping
    // Will be applied in BeginFrame/EndFrame
}

ZepFont& ZepDisplay_Raylib::GetFont(ZepTextType type)
{
    if (m_fonts[(int)type] == nullptr)
    {
        if (m_spDefaultFont == nullptr)
        {
            m_spDefaultFont = std::make_shared<ZepFont_Raylib>(*this, m_defaultFont, 16);
        }
        return *m_spDefaultFont;
    }
    return *m_fonts[(int)type];
}

bool ZepDisplay_Raylib::ShouldClose() const
{
    return WindowShouldClose();
}

void ZepDisplay_Raylib::BeginFrame()
{
    BeginDrawing();

    // Clear with dark background (like a terminal)
    ClearBackground({ 26, 26, 26, 255 });

    // Apply clipping if enabled
    if (m_clipEnabled)
    {
        BeginScissorMode((int)m_clipRect.x, (int)m_clipRect.y,
            (int)m_clipRect.w, (int)m_clipRect.h);
    }
}

void ZepDisplay_Raylib::EndFrame()
{
    if (m_clipEnabled)
    {
        EndScissorMode();
        m_clipEnabled = false;
    }
    EndDrawing();
}

void ZepDisplay_Raylib::ClearBackground(const NVec4f& col)
{
    Color c = {
        (unsigned char)(col.x),
        (unsigned char)(col.y),
        (unsigned char)(col.z),
        (unsigned char)(col.w)
    };
    ::ClearBackground(c);
}

int ZepDisplay_Raylib::GetKeyPressed()
{
    return ::GetKeyPressed();
}

int ZepDisplay_Raylib::GetCharPressed()
{
    return ::GetCharPressed();
}

bool ZepDisplay_Raylib::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool ZepDisplay_Raylib::IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

Vector2 ZepDisplay_Raylib::GetMousePosition()
{
    return ::GetMousePosition();
}

} // namespace Zep