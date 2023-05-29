#include "AUIFactory.h"

namespace TL_UI
{
    APanel* AUIFactory::CreatePanel()
    {
        const auto _panel = CreatePanel_Impl();
        return _panel;
    }
}
