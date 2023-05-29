#include "AUserInterface.h"
#include "Common_min.h"

namespace TL_UI
{
    void AUserInterface::Initialize(TL_Application::ApplicationContext* _appContext)
    {
        m_UIFactory = std::unique_ptr<AUIFactory>(CreateUIFactory_Impl());
        m_UIDrawer = std::unique_ptr<AUIDrawer>(CreateUIDrawer_Impl());
    }

    APanel* AUserInterface::CreatePanel()
    {
        const auto _panel = m_UIFactory->CreatePanel();
        return StoreAndReturnPanel(_panel);
    }

    APanel* AUserInterface::StoreAndReturnPanel(APanel* _panel)
    {
        const auto _iter = std::find_if(
            m_Panels.begin(), m_Panels.end()
            , [_panel](std::unique_ptr<APanel>& p) { return p.get() == _panel; });
        assert(_iter == m_Panels.end());
        m_Panels.emplace_back(_panel);
        return _panel;
    }
}
