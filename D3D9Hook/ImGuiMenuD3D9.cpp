#include "ImGuiMenuD3D9.h"

ImGuiMenu::ImGuiMenu()
{
    bSetup          = false;
    bShowMenu       = false;
    bShowCursor     = false;
    hWindow         = nullptr;
}

ImGuiMenu::~ImGuiMenu()
{
    CleanUp();
}

void ImGuiMenu::SetupImGui(HWND hWindow, IDirect3DDevice9* pDevice)
{
    if (!bSetup)
    {
        // Store window menu is drawn on
        this->hWindow = hWindow;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(hWindow);
        ImGui_ImplDX9_Init(pDevice);

        // Shows mouse cursor
        ImGui::GetIO().MouseDrawCursor = true;

        // Sets up input for menu
        SetupMenuInput(hWindow);

        bSetup = true;
    }
}

void ImGuiMenu::SetupMenuInput(HWND hWindow)
{
    // Sets up input hook
    oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)WndProc);
}

void ImGuiMenu::CleanUp()
{
    // Clean up ImGui
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Remove input hook
    (WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);

    bSetup = false;
    bShowMenu = false;
    bShowCursor = false;
    hWindow = nullptr;
}

//-----------------------------NOTE-----------------------------------//
//  Turn this to a function pointer in the future...                  //
//  Or keeps as a demo window unless function pointer is given        //
//-----------------------------NOTE-----------------------------------//
void ImGuiMenu::Menu()
{
    if (bShowMenu)
    {
        ImGui::GetIO().MouseDrawCursor = true;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu");

        /*                      EXAMPLE OF MENU
        ImGui::SliderFloat("Field of View", (float*)(*cg_fov + 0x18), 35.f, 120.f);
        ImGui::Checkbox("Third Person", (bool*)(*cg_thirdPerson + 0x18));

        ImGui::SliderFloat("Gun X", (float*)(*cg_gun_x + 0x18), -50.f, 50.f);
        ImGui::SliderFloat("Gun Y", (float*)(*cg_gun_y + 0x18), -50.f, 50.f);
        ImGui::SliderFloat("Gun Z", (float*)(*cg_gun_z + 0x18), -50.f, 50.f);
        */

        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
    else
    {
        ImGui::GetIO().MouseDrawCursor = false;
    }
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}