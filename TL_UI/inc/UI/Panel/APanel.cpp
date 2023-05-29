#include "APanel.h"
#include "Common_min.h"
#include "UI/Widget/AWidget.h"

namespace TL_UI
{
    void APanel::Draw()
    {
        for (const auto& _w : m_Widgets)
        {
            _w->Draw();
        }
    }
}
