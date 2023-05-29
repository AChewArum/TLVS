#pragma once

#include "Common_min.h"
#include "Application/internal/dll.h"

namespace TL_GameEngine
{
    class Resources;
}

namespace TL_UI
{
    class AUserInterface;
}

namespace TL_Graphics
{
    class RenderSystem;
}

namespace TL_Application
{
    class GameTime;
    class Input;
    class Window;
}

using WndProcEvent = TL_Common::Event<HWND, UINT, WPARAM, LPARAM>;

namespace TL_Application
{
    struct APPLICATION_API ApplicationContextPreparation
    {
        // Application 생성 설정입니다.

        bool bEditor;
        bool bHeadless;

        // Window 객체 생성에 필요한 매개변수입니다.

        int nCmdShow;
        HINSTANCE hInstance;
        const tstring& windowClassName;
        const tstring& windowName;
        UINT width;
        UINT height;
    };

    class APPLICATION_API ApplicationContext
    {
    private:
        ApplicationContext();

    public:
        virtual ~ApplicationContext();

        static std::unique_ptr<ApplicationContext> Create(ApplicationContextPreparation _preparation);
        static void Release(ApplicationContext* _appContext);

        void PreTick();
        void PreRenderObjects();
        void PostRenderObjects();
        void Present();

    private:
        void ParseCommandLineArgs();

        static TL_UI::AUserInterface* CreateUI(
            ApplicationContext* _appContext
            , const tstring& _uiType
        );

    private:
        std::map<std::string, std::string> m_CommandLineArgs;
        bool m_bEditor;

        TL_Application::Window* m_Window;
        std::unique_ptr<TL_Application::Input> m_Input;
        std::unique_ptr<TL_Application::GameTime> m_GameTime;
        TL_Graphics::RenderSystem* m_RenderSystem;

        bool m_bWindowFocus;

    public:
        const auto& GetCommandLineArgs() const { return m_CommandLineArgs; }
        TL_Application::Window* GetWindow() const { return m_Window; }
        TL_Application::Input* GetInput() const { return m_Input.get(); }
        TL_Application::GameTime* GetGameTime() const { return m_GameTime.get(); }
        TL_Graphics::RenderSystem* GetRenderSystem() const { return m_RenderSystem; }

        bool IsEditor() const { return m_bEditor; }
        bool IsHeadless() const { return m_Window == nullptr; }
        bool IsWindowFocus() const { return m_bWindowFocus; }
    };
}
