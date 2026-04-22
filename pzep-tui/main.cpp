#include "zep/ansi/display_ansi.h"
#include "zep/editor.h"
#include "zep/mode_vim.h"
#include "zep/tab_window.h"
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

namespace Zep
{
class ZepFont_ANSI : public ZepFont
{
public:
    ZepFont_ANSI(ZepDisplay& display)
        : ZepFont(display)
    {
        m_pixelHeight = 16;
        m_defaultCharSize = NVec2f(8.0f, 16.0f);
    }

    virtual void SetPixelHeight(int height) override
    {
        m_pixelHeight = height;
        m_defaultCharSize.y = (float)height;
    }

    virtual NVec2f GetTextSize(const uint8_t* pBegin, const uint8_t* pEnd = nullptr) const override
    {
        if (pEnd == nullptr)
        {
            pEnd = pBegin + strlen((const char*)pBegin);
        }
        auto count = pEnd - pBegin;
        return NVec2f(float(count) * m_defaultCharSize.x, m_defaultCharSize.y);
    }

    virtual const NVec2f& GetDefaultCharSize() override
    {
        return m_defaultCharSize;
    }

    virtual const NVec2f& GetDotSize() override
    {
        m_dotSize = NVec2f(m_defaultCharSize.x * 0.1f, m_defaultCharSize.y * 0.1f);
        return m_dotSize;
    }

    virtual void BuildCharCache() override
    {
        m_charCacheDirty = false;
        for (int i = 0; i < 256; i++)
        {
            m_charCacheASCII[i] = m_defaultCharSize;
        }
    }

    virtual void InvalidateCharCache() override
    {
        m_charCacheDirty = true;
    }

    virtual NVec2f GetCharSize(const uint8_t* pChar)
    {
        return m_defaultCharSize;
    }
};
} // namespace Zep

class ZepTerminal : public Zep::IZepComponent
{
public:
    ZepTerminal()
        : m_display(nullptr)
        , m_pEditor(nullptr)
        , m_running(true)
        , m_inited(false)
    {
    }

    ~ZepTerminal()
    {
        if (m_inited)
        {
            m_pEditor->UnRegisterCallback(this);
            delete m_pEditor;
        }
    }

    virtual Zep::ZepEditor& GetEditor() const override
    {
        return *m_pEditor;
    }

    void Init(Zep::ZepDisplay_ANSI* display)
    {
        m_display = display;

        m_pEditor = new Zep::ZepEditor(m_display, ".", 0, nullptr);
        m_pEditor->RegisterCallback(this);
        m_pEditor->SetGlobalMode(Zep::ZepMode_Vim::StaticName());
        m_inited = true;
    }

    void SetDisplayRegion()
    {
        int width = m_display->GetTerminalWidth();
        int height = m_display->GetTerminalHeight();

        auto charWidth = m_display->GetFont(Zep::ZepTextType::Text).GetDefaultCharSize().x;
        auto charHeight = m_display->GetFont(Zep::ZepTextType::Text).GetDefaultCharSize().y;

        Zep::NVec2f topLeft(0.0f, charHeight);
        Zep::NVec2f bottomRight(width * charWidth, height * charHeight);
        m_pEditor->SetDisplayRegion(topLeft, bottomRight);
    }

    void OpenFile(const std::string& filename)
    {
        if (filename.empty())
        {
            m_pEditor->InitWithText("Untitled", "");
        }
        else
        {
            m_pEditor->InitWithFile(filename);
        }
    }

    virtual void Notify(std::shared_ptr<Zep::ZepMessage> message) override
    {
        if (message->messageId == Zep::Msg::HandleCommand)
        {
            std::string cmd = message->str;
            if (cmd == ":q" || cmd == ":x" || cmd == ":wq")
            {
                m_running = false;
            }
            else if (cmd == ":q!")
            {
                m_running = false;
            }
            message->handled = true;
            return;
        }
        message->handled = false;
    }

    void Run()
    {
        SetDisplayRegion();

        while (m_running)
        {
            SetDisplayRegion();

            m_display->BeginFrame();
            m_pEditor->Display();
            m_display->EndFrame();

            std::this_thread::sleep_for(std::chrono::milliseconds(16));

            int key = m_display->GetKeyPressed();
            if (key > 0)
            {
                HandleKey(key);
            }
        }

        printf("\x1b[2J\x1b[H\x1b[0m");
        printf("Goodbye!\n");
    }

    void HandleKey(int key)
    {
        if (m_pEditor->GetActiveTabWindow() == nullptr)
            return;
        auto pWindow = m_pEditor->GetActiveTabWindow()->GetActiveWindow();
        if (pWindow == nullptr)
            return;

        auto pMode = pWindow->GetBuffer().GetMode();
        if (pMode == nullptr)
            return;

        uint32_t mod = 0;

        if (key == 27)
        {
            pMode->AddKeyPress(Zep::ExtKeys::ESCAPE, mod);
        }
        else if (key == 13)
        {
            pMode->AddKeyPress(Zep::ExtKeys::RETURN, mod);
        }
        else if (key == 127 || key == 8)
        {
            pMode->AddKeyPress(Zep::ExtKeys::BACKSPACE, mod);
        }
        else if (key == 9)
        {
            pMode->AddKeyPress(Zep::ExtKeys::TAB, mod);
        }
        else if (key == 224)
        {
            int next = m_display->GetKeyPressed();
            if (next == 72)
            {
                pMode->AddKeyPress(Zep::ExtKeys::LEFT, mod);
            }
            else if (next == 77)
            {
                pMode->AddKeyPress(Zep::ExtKeys::RIGHT, mod);
            }
            else if (next == 80)
            {
                pMode->AddKeyPress(Zep::ExtKeys::UP, mod);
            }
            else if (next == 75)
            {
                pMode->AddKeyPress(Zep::ExtKeys::DOWN, mod);
            }
            else if (next == 83)
            {
                pMode->AddKeyPress(Zep::ExtKeys::DEL, mod);
            }
            else if (next == 71)
            {
                pMode->AddKeyPress(Zep::ExtKeys::HOME, mod);
            }
            else if (next == 79)
            {
                pMode->AddKeyPress(Zep::ExtKeys::END, mod);
            }
        }
        else if (key >= 32 && key < 127)
        {
            pMode->AddKeyPress(key, mod);
        }
    }

    Zep::ZepDisplay_ANSI* m_display;
    Zep::ZepEditor* m_pEditor;
    bool m_running;
    bool m_inited;
};

static ZepTerminal* g_pTerminal = nullptr;

static void SignalHandler(int signal)
{
    if (g_pTerminal)
    {
        g_pTerminal->m_running = false;
    }
}

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }

    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    printf("\x1b[2J\x1b[H");
    printf("\x1b[96mpZep Terminal Editor\x1b[0m\n");
    printf("Vim-like editor. Commands: :w (save) :q (quit)\n");
    printf("Keys: i=insert, ESC=normal, h/j/k/l=navigate\n");
    printf("---\n");

    Zep::ZepDisplay_ANSI display;
    ZepTerminal terminal;
    g_pTerminal = &terminal;

    terminal.Init(&display);
    terminal.OpenFile(filename);
    terminal.Run();

    g_pTerminal = nullptr;
    return 0;
}