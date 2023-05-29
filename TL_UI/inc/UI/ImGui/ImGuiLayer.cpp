#include "ImGuiLayer.h"

#include "Common_min.h"

#include "imgui.h"
#include "ImGuiUIDrawer.h"
#include "ImGuiUIFactory.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "Application/ApplicationContext.h"
#include "Application/Window.h"
#include "Graphics/RenderSystem.h"
#include "Panel/ImGuiPanel.h"
#include "UI/Widget/ATreeView.h"
#include "Widget/ImGuiAlternativeRowBackgroundList.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TL_UI
{
    void ImGuiLayer::Initialize(TL_Application::ApplicationContext* _appContext)
    {
        AUserInterface::Initialize(_appContext);

        m_WndProcEventHandler = [this](HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
        {
            ::ImGui_ImplWin32_WndProcHandler(_hWnd, _message, _wParam, _lParam);

            switch (_message)
            {
                case WM_DPICHANGED:
                {
                    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
                    {
                        //const int dpi = HIWORD(wParam);
                        //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
                        const RECT* suggested_rect = (RECT*)_lParam;
                        ::SetWindowPos(_hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left,
                                       suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
                    }
                    break;
                }
            }
        };
        _appContext->GetWindow()->WndProcEvent.AddListener(m_WndProcEventHandler);

        // Setup Dear ImGui context"

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        //io.ConfigViewportsNoDefaultParent = true;
        //io.ConfigDockingAlwaysTabBar = true;
        //io.ConfigDockingTransparentPayload = true;
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(_appContext->GetWindow()->GetHWnd());
        ImGui_ImplDX11_Init(_appContext->GetRenderSystem()->GetDevice(), _appContext->GetRenderSystem()->GetDeviceContext());
    }

    void ImGuiLayer::Release()
    {
        // m_WindowMessageEvent->RemoveListener(m_WindowHandler);

        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::PreTick()
    {
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::PostTick()
    {
        for (const auto& _panel : m_Panels)
        {
            _panel->Draw();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    AUIFactory* ImGuiLayer::CreateUIFactory_Impl()
    {
        return new ImGuiUIFactory();
    }

    AUIDrawer* ImGuiLayer::CreateUIDrawer_Impl()
    {
        return new ImGuiUIDrawer();
    }

    void ImGuiLayer::SampleTick()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }
}
