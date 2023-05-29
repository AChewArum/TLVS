\
#pragma once

#include "UI/AUserInterface.h"
#include "Common_min.h"
#include "UI/Widget/AWidget.h"

using WndProcEvent = TL_Common::Event<HWND, UINT, WPARAM, LPARAM>;

namespace TL_UI
{
    class UI_API ImGuiLayer :
        public AUserInterface
    {
    public:
        ~ImGuiLayer() override = default;

        void Initialize(TL_Application::ApplicationContext* _appContext) override;
        void Release() override;
        void PreTick() override;
        void PostTick() override;

    protected:
        AUIFactory* CreateUIFactory_Impl() override;
        AUIDrawer* CreateUIDrawer_Impl() override;

    private:
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
