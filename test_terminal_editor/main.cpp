#include "zep/ansi/display_ansi.h"
#include "zep/editor.h"
#include "zep/mode_vim.h"
#include "zep/tab_window.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

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

    void DisplayStatus()
    {
        printf("\x1b[%d;1H", m_display->GetTerminalHeight());
        printf("\x1b[K");
        printf("\x1b[90mNORMAL\x1b[0m");
    }

    void Run()
    {
        while (m_running)
        {
            m_display->BeginFrame();
            m_pEditor->Display();
            DisplayStatus();
            m_display->EndFrame();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));

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
        if (key == 'q' || key == 'Q')
        {
            m_running = false;
            return;
        }

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
        else if (key >= 32 && key < 127)
        {
            pMode->AddKeyPress(key, mod);
        }
        else if (key == 127 || key == 8)
        {
            pMode->AddKeyPress(Zep::ExtKeys::BACKSPACE, mod);
        }
    }

    Zep::ZepDisplay_ANSI* m_display;
    Zep::ZepEditor* m_pEditor;
    bool m_running;
    bool m_inited;
};

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }

    printf("\x1b[2J\x1b[H");
    printf("\x1b[96mpZep Terminal Editor\x1b[0m\n");
    printf("Commands: :w (save) :q (quit) :wq (save+quit)\n");
    printf("---\n");

    Zep::ZepDisplay_ANSI display;
    ZepTerminal terminal;
    terminal.Init(&display);
    terminal.OpenFile(filename);
    terminal.Run();

    return 0;
}