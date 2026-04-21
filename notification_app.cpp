// Zep + ImGui Notification Center Integration
// Compile after building Zep library
// Link: Zep, ImGui, SDL2, OpenGL

#include <memory>
#include <string>
#include <vector>

// Forward declare SDL types (actual SDL2 headers included in build)
struct SDL_Window;
struct SDL_GLContext;

namespace ImGui
{
struct ImVec2;
struct ImVec4;
class ImGuiContext;
class ImFontAtlas;
} // namespace ImGui

// Zep Editor API
namespace Zep
{

class ZepEditor_ImGui;
class ZepBuffer;
class ZepTabWindow;
class ZepWindow;
class ZepDisplay_ImGui;

struct ZepFont_ImGui;

} // namespace Zep

struct NotificationItem
{
    std::string source;
    std::string message;
    std::string timestamp;
    enum class Status
    {
        Pass,
        Fail,
        Info
    } status;
};

class NotificationApp
{
public:
    NotificationApp();
    ~NotificationApp();

    bool Init(int width, int height);
    void Render();
    void Shutdown();
    bool IsRunning() const;

    // Add notification to the feed
    void AddNotification(NotificationItem::Status status,
        const std::string& source,
        const std::string& message);

private:
    void RenderNotificationPanel(float width, float height);
    void RenderEditorPanel(float x, float y, float width, float height);
    void RenderStatusBar(float width);

    std::unique_ptr<Zep::ZepEditor_ImGui> spEditor;
    std::vector<NotificationItem> notifications;
    bool running = false;
};

// Implementation -------------------------------------------------------------

NotificationApp::NotificationApp() = default;
NotificationApp::~NotificationApp() = default;

bool NotificationApp::Init(int width, int height)
{
    (void)width;
    (void)height;

    // Create Zep editor instance
    // Config stored in ./zep_config directory
    std::string configPath = "./zep_config";
    spEditor = std::make_unique<Zep::ZepEditor_ImGui>(configPath);

    // Initialize with sample notification config
    spEditor->InitWithText("notifications.toml", R"(
# Notification Configuration
# Edit this file to customize your feeds

[feeds.github]
enabled = true
events = ["push", "pr", "workflow"]
channels = ["#dev"]

[feeds.jenkins]
url = "http://jenkins:8080"
jobs = ["main-build", "tests", "deploy"]
on_failure = "urgent"

[feeds.custom]
script = "python scripts/check_feeds.py"

[filters]
error = ["ERROR", "FAIL", "CRITICAL"]
warning = ["WARN", "deprecated"]
)");

    // Add some sample notifications
    notifications = {
        { "GitHub", "✓ main branch build passed", "2m ago", NotificationItem::Status::Pass },
        { "Jenkins", "✗ build #4521 failed", "5m ago", NotificationItem::Status::Fail },
        { "Deploy", "✓ v2.1.0 → staging", "15m ago", NotificationItem::Status::Pass },
    };

    running = true;
    return true;
}

void NotificationApp::Render()
{
    // Main window dimensions (would come from SDL/ImGui in real app)
    float mainWidth = 1000.0f;
    float mainHeight = 700.0f;

    // Left: Notification panel (30% width)
    float notifWidth = mainWidth * 0.3f;
    RenderNotificationPanel(notifWidth, mainHeight - 50);

    // Right: Editor panel (remaining width)
    float editorX = notifWidth + 20;
    float editorWidth = mainWidth - notifWidth - 30;
    RenderEditorPanel(editorX, 10, editorWidth, mainHeight - 60);

    // Bottom: Status bar
    RenderStatusBar(mainWidth);
}

void NotificationApp::RenderNotificationPanel(float width, float height)
{
    // Pseudo-ImGui calls - replace with actual ImGui in your app
    (void)width;
    (void)height;

    // In real ImGui:
    // ImGui::SetNextWindowPos(ImVec2(10, 10));
    // ImGui::SetNextWindowSize(ImVec2(width, height));
    // ImGui::Begin("Notifications");

    for (const auto& notif : notifications)
    {
        // Color by status
        // Pass: green, Fail: red, Info: blue
        // ImGui::TextColored(color, "[%s] %s: %s",
        //     notif.status == NotificationItem::Status::Pass ? "✓" :
        //     notif.status == NotificationItem::Status::Fail ? "✗" : "ℹ",
        //     notif.source.c_str(),
        //     notif.message.c_str());
        //
        // ImGui::SameLine();
        // ImGui::TextColored(gray, notif.timestamp.c_str());
    }
    // ImGui::End();
}

void NotificationApp::RenderEditorPanel(float x, float y, float width, float height)
{
    // Zep editor rendering
    // In real ImGui integration:
    // ImGui::SetNextWindowPos(ImVec2(x, y));
    // ImGui::SetNextWindowSize(ImVec2(width, height));
    // ImGui::Begin("Editor");

    // The Zep editor window is rendered by Zep internally
    // when you call spEditor->Render() or similar

    (void)x;
    (void)y;
    (void)width;
    (void)height;
}

void NotificationApp::RenderStatusBar(float width)
{
    // Bottom status bar
    // ImGui::SetNextWindowPos(ImVec2(10, height - 40));
    // ImGui::SetNextWindowSize(ImVec2(width, 30));
    // ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar);
    //
    // ImGui::Text("Zep Editor | Mode: %s | Buffer: notifications.toml",
    //     spEditor->GetActiveTabWindow()->GetActiveWindow()->GetMode()->GetName().c_str());
    //
    // ImGui::End();
    (void)width;
}

void NotificationApp::Shutdown()
{
    notifications.clear();
    spEditor.reset();
}

bool NotificationApp::IsRunning() const
{
    return running;
}

void NotificationApp::AddNotification(NotificationItem::Status status,
    const std::string& source,
    const std::string& message)
{
    NotificationItem item;
    item.status = status;
    item.source = source;
    item.message = message;
    item.timestamp = "just now";

    notifications.insert(notifications.begin(), item);

    // Keep only last 100 notifications
    if (notifications.size() > 100)
    {
        notifications.resize(100);
    }
}

// Main entry point (pseudo-code for SDL2 + ImGui loop) --------------------------------

/*
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // SDL2 + OpenGL + ImGui init (from your existing demo)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    // ... setup window, GL context, ImGui ...

    NotificationApp app;
    app.Init(1024, 768);

    while (app.IsRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) break;
        }

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Render our app
        app.Render();

        ImGui::Render();
        // GL draw ...

        SDL_Delay(16);
    }

    app.Shutdown();
    // cleanup SDL, ImGui ...
    return 0;
}
*/

int main()
{
    // Quick test - just init and print status
    NotificationApp app;
    app.Init(1024, 768);

    // Print what's in the editor buffer
    auto* buffer = app.spEditor->GetActiveTabWindow()
                       ->GetActiveWindow()
                       ->GetBuffer();

    printf("Editor initialized with buffer: %s\n",
        buffer ? "OK" : "NULL");
    printf("Notifications loaded: %zu\n",
        app.notifications.size());

    app.Shutdown();
    return 0;
}