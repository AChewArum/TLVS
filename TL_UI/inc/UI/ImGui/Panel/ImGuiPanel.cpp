#include "ImGuiPanel.h"

#include "UI/ImGui/Widget/ImGuiAlternativeRowBackgroundList.h"
#include "Common/StringHelper.h"

namespace TL_UI
{
    ImGuiPanel::ImGuiPanel(AUIFactory* _factory) : APanel(_factory)
    {
        static int _panelIndex = 0;
        m_Name = StringHelper::ToString(StringHelper::Format(TEXT("panel_%d"), _panelIndex++));
    }

    void ImGuiPanel::Draw()
    {
        m_Context.Reset();

        ImGui::Begin(m_Name.c_str(), &m_bOpen);

        for (const auto& _w : m_Widgets)
        {
            _w->Draw();
        }

        ImGui::End();
    }
}
