#pragma once

#include "UI/Widget/AAlternativeRowBackgroundList.h"
#include "imgui.h"

namespace TL_UI
{
    class UI_API ImGuiAlternativeRowBackgroundList :
        public AAlternativeRowBackgroundList
    {
    protected:
        void Draw_Impl() override;

    private:
        void DrawRowsBackground(
            std::vector<std::string>& _items
            , float _lineHeight
            , float _startX, float _endX, float _startY
            , ImU32 _evenStyle, ImU32 _oddStyle
        );
    };
}
