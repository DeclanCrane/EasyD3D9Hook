#include <Windows.h>
#include <iostream>

#include "NullSystem.h"
#include "Console.h"
#include "D3D9Hook.h"
#include "ImGuiMenuD3D9.h"

ImGuiMenu Menu;

HRESULT WINAPI EndSceneCallback(LPDIRECT3DDEVICE9 d3dDevice) {
    if(Menu.bSetup)
        Menu.Menu();
    return 0;
}

HRESULT WINAPI ResetCallback(LPDIRECT3DDEVICE9 d3dDevice, D3DPRESENT_PARAMETERS* d3dpp) {
    std::cout << "Reset!\n";
    return 0;
}

int MainThread(PVOID pModule)
{
    Console console;
    console.CreateConsole();

    HWND hWnd = GetCurrentProcParentWindow();
    std::cout << "GetCurrentProcessWindow: " << hWnd << "\n";

    int windowTextLength = GetWindowTextLength(hWnd);
    WCHAR text[256] = L"";
    GetWindowTextW(hWnd, text, windowTextLength + 1);

    std::cout << "ProcessID: " << GetCurrentProcessId() << "\n";

    std::wcout << "Window Name: " << text << "\n";

    D3D9Hook D3D9(EndSceneCallback, ResetCallback);
    if (!D3D9.Install())
        std::cout << "Big fail\n";

    Menu.SetupImGui(hWnd, D3D9.GetDevice());

    while (true) {
        // Eject
        if (GetAsyncKeyState(VK_END) & 0x01) {
            break;
        }

        // If application window is lost or reset
        if (D3D9.HasWindowBeenLost()) {
            Menu.CleanUp();
            D3D9.Restore();
            std::cout << "Device: " << D3D9.GetDevice() << "\n";
            Menu.SetupImGui(D3D9.GetWindow(), D3D9.GetDevice());
        }

        // Toggle Menu
        if (GetAsyncKeyState(VK_INSERT) & 0x01) {
            Menu.bShowMenu = !Menu.bShowMenu;
        }
    }
    Menu.CleanUp();
    D3D9.Uninstall();
    console.CloseConsole();
    FreeLibraryAndExitThread((HMODULE)pModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, (PVOID)hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

