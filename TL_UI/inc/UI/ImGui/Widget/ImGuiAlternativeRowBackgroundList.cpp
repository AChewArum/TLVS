#include "ImGuiAlternativeRowBackgroundList.h"

namespace TL_UI
{
    void ImGuiAlternativeRowBackgroundList::Draw_Impl()
    {
        const float _startX = ImGui::GetWindowPos().x;
        const float _endX = _startX + ImGui::GetWindowSize().x;
        const float _itemSpacingY = ImGui::GetStyle().ItemSpacing.y;
        const float _startY = ImGui::GetCursorScreenPos().y + -_itemSpacingY * 0.5f;
        const float _lineHeight = ImGui::GetTextLineHeight() + _itemSpacingY;

        DrawRowsBackground(
            m_ItemList
            , _lineHeight
            , _startX, _endX
            , _startY
            , 0, ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, 0.5f))
        );
    }

    void ImGuiAlternativeRowBackgroundList::DrawRowsBackground(
        std::vector<std::string>& _items
        , float _lineHeight
        , float _startX, float _endX, float _startY
        , ImU32 _evenStyle, ImU32 _oddStyle
    )
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        int row_display_start;
        int row_display_end;
        ImGui::CalcListClipping(_items.size(), _lineHeight, &row_display_start, &row_display_end);
        for (int row_n = row_display_start; row_n < row_display_end; row_n++)
        {
            ImU32 col = (row_n & 1) ? _oddStyle : _evenStyle;
            if ((col & IM_COL32_A_MASK) == 0)
                continue;
            float y1 = _startY + (_lineHeight * row_n);
            float y2 = y1 + _lineHeight;
            draw_list->AddRectFilled(ImVec2(_startX, y1), ImVec2(_endX, y2), col);
        }

        for (int n = 0; n < _items.size(); n++)
            ImGui::Text(_items[n].c_str(), n);
    }
}
