#pragma once

namespace TL_Application
{
    class ApplicationContext;
}

namespace TL_GameEditor
{
    class ImGuiLayer
    {
    public:
        void Initialize(TL_Application::ApplicationContext* _appContext);
        void Release();

        void PreRender();
        void PostRender();
        
        /**
         * @brief 임시적으로 작성하는 샘플 Tick입니다.
         */
        void SampleTick();

    private:
        WndProcEvent::THandler m_WndProcEventHandler;

        bool show_demo_window = true;
        bool show_another_window = false;
    };
}