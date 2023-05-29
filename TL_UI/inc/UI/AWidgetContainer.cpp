#include "AWidgetContainer.h"
#include "Common_min.h"

namespace TL_UI
{
    AWidget* AWidgetContainer::StoreAndReturnWidget(AWidget* _widget)
    {
        const auto _iter = std::find_if(
            m_Widgets.begin(), m_Widgets.end()
            , [_widget](std::unique_ptr<AWidget>& w) { return w.get() == _widget; });
        assert(_iter == m_Widgets.end());
        m_Widgets.emplace_back(_widget);
        return _widget;
    }
}
