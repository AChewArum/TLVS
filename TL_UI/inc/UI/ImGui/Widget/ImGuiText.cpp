#include "ImGuiText.h"

#include "imgui.h"

namespace TL_UI
{
    void ImGuiText::Draw_Impl()
    {
        ImGui::Text(m_Data.c_str());
    }
}
