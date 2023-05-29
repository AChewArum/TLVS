#pragma once

#include "UI/Widget/ATreeView.h"

namespace TL_UI
{
    class ImGuiTreeView :
        public ATreeView
    {
    protected:
        void DrawNodeRecursive_Impl(Target* _data) override;
    };
}
