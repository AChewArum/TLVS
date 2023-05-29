#pragma once

#include <memory>
#include <vector>

#include "UI/internal/dll.h"
#include "UI/Panel/APanel.h"
#include "UI/AUIFactory.h"
#include "UI/AUIDrawer.h"

namespace TL_Application
{
    class ApplicationContext;
}

namespace TL_UI
{
    class UI_API AUserInterface
    {
    public:
        AUserInterface() = default;
        virtual ~AUserInterface() = default;

        AUserInterface(const AUserInterface& _other) = delete;
        AUserInterface(AUserInterface&& _other) noexcept = delete;
        AUserInterface& operator=(const AUserInterface& _other) = delete;
        AUserInterface& operator=(AUserInterface&& _other) noexcept = delete;

        virtual void Initialize(TL_Application::ApplicationContext* _appContext);
        virtual void Release() = 0;
        virtual void PreTick() = 0;
        virtual void PostTick() = 0;

        APanel* CreatePanel();

    protected:
        virtual AUIFactory* CreateUIFactory_Impl() = 0;
        virtual AUIDrawer* CreateUIDrawer_Impl() = 0;

        APanel* StoreAndReturnPanel(APanel* _panel);

        std::unique_ptr<AUIFactory> m_UIFactory;
        std::unique_ptr<AUIDrawer> m_UIDrawer;
        std::vector<std::unique_ptr<APanel>> m_Panels;

    public:
        AUIFactory* GetUIFactory() const { return m_UIFactory.get(); }
        AUIDrawer* GetUIDrawer() const { return m_UIDrawer.get(); }
    };
}
