#include <Windows.h>
#include <string>
#include <iostream>
#include <TlHelp32.h>
#include <d3d11.h>
#include <Overlay/Overlay.hpp>
#include <Functions/Console.hpp>
#include <Interfaces/Loader.hpp>
#include <Memory/Memory.hpp>
#include <Memory/Handle.hpp>
#include <Overlay/ExeConfig.hpp>

HANDLE Memory::ProcessHandle = NULL;

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
    // Always show console for prompt
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    SetConsoleTitleW(L"Cs2 External");

    SetConsoleTextColor(ImColor(255, 255, 0));
    std::cout << "[!] Please open CS2..." << std::endl;

    // Wait for CS2 to start
    while ((CS2::ProcID = PM.GetProcessID(L"cs2.exe")) == 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    system("cls"); // Clear console
    SetConsoleTextColor(ImColor(0, 255, 0));
    std::cout << "[+] CS2 detected! PID: " << CS2::ProcID << "\n" << std::endl;

    // Initialize CS2 module base addresses
    CS2::Initialize();

    if (FileExists("settings.json"))
        LoadSettings("settings.json");

    if (Flags::g_showConsole)
    {
        CreateConsoleWindow(L"Console");
        ShowTitle();
    }

    mem.initdriver(CS2::ProcID);

    HANDLE SteamHandle = GetProcessHandle(L"steam.exe");
    if (!SteamHandle)
        return 1;

    Memory::ProcessHandle = GetCS2HandleFrom(SteamHandle);
    if (!Memory::ProcessHandle)
        return 1;

    HWND window = InitializeWindow(instance, cmd_show);
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* device_context = nullptr;
    IDXGISwapChain* swap_chain = nullptr;
    ID3D11RenderTargetView* render_target_view = nullptr;

    if (!InitializeDirectX(window, &device, &device_context, &swap_chain, &render_target_view))
        return 1;

    InitializeImGui(window, device, device_context);

    RunMainLoop(window, device_context, swap_chain, render_target_view);
    CleanUp(swap_chain, device, device_context, render_target_view, window, {});

    return 0;
}
