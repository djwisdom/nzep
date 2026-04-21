// Simple Terminal Notification Test
// No dependencies - pure C++17
// Works on: Windows, Linux, FreeBSD

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "../notifications.h"
#include "../notifications_term.h"

using namespace ZepNotifications;

void PrintBanner()
{
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║     ZEP NOTIFICATION TERMINAL TEST         ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

void DemoNotifications()
{
    TerminalNotificationRenderer renderer;
    NotificationManager mgr;

    std::cout << "[1] Adding sample notifications...\n\n";

    // Add demo notifications
    mgr.Add(BuildFailed(
        "core-lib", "all", "undefined reference",
        "src/util.cpp:128", "1234", "http://jenkins/build/1234")
            .Build());

    mgr.Add(TestFailed(
        "auth-suite", "test_login: expected 200 got 500",
        "http://jenkins/test/456")
            .Build());

    mgr.Add(RuntimeError(
        "auth-service", "NullPointerException",
        "AuthController.login()", "req-abc123",
        "http://trace/service/abc123")
            .Build());

    mgr.Add(DeployComplete(
        "staging", "v2.1.0", true,
        "http://deploy/staging/789")
            .Build());

    mgr.Add(SecurityAlert(
        "lib/utils.js", NotificationSeverity::High,
        "update lodash@>4.5.0",
        "http://gh/advisory/789")
            .Build());

    std::cout << "Notifications added: " << mgr.Count() << "\n";
    std::cout << "Critical: " << mgr.GetCritical().size() << "\n\n";

    // Show platform
    std::cout << "Platform: " << TerminalStyle::GetPlatformStyle() << "\n";
    std::cout << "ANSI Support: " << (renderer.HasANSI() ? "Yes" : "No") << "\n\n";

    // Get terminal size
    int w, h;
    TerminalNotificationRenderer::GetTermSize(w, h);
    std::cout << "Terminal: " << w << "x" << h << "\n\n";

    // Display panel
    std::cout << "Press Enter to see full panel display...\n";
    std::cin.get();

    renderer.RenderPanel(mgr, w, h);
}

void RunInteractive()
{
    TerminalNotificationApp app;
    app.Init();
    app.Run();
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    PrintBanner();

    std::cout << "Choose test:\n";
    std::cout << "  [1] Quick demo (print notifications)\n";
    std::cout << "  [2] Interactive mode (arrow keys + Enter)\n";
    std::cout << "  [3] Run all unit tests\n";
    std::cout << "\n> ";

    int choice = '1';
    std::cin >> choice;
    std::cin.ignore(); // consume newline

    switch (choice)
    {
    case 1:
        std::cout << "\n=== QUICK DEMO ===\n\n";
        DemoNotifications();
        break;

    case 2:
        std::cout << "\n=== INTERACTIVE MODE ===\n";
        std::cout << "Controls: [C]ritical, [B]uild, [T]est, [Q]uit\n\n";
        RunInteractive();
        break;

    case 3:
        std::cout << "\n=== RUNNING UNIT TESTS ===\n\n";
        // Note: gtest needs to be linked for this
        // If built with tests, would run them here
        std::cout << "(Run ./unittests.exe --gtest_filter='Notification*' for tests)\n";
        break;

    default:
        std::cout << "Invalid choice, running demo...\n";
        DemoNotifications();
        break;
    }

    std::cout << "\n=== DONE ===\n";
    std::cout << "\nPress Enter to exit...\n";
    std::cin.get();

    return 0;
}