#pragma once
#include "UI/Widget/AText.h"

namespace TL_UI
{
    class ImGuiText :
        public AText
    {
    protected:
        void Draw_Impl() override;
    };
}
