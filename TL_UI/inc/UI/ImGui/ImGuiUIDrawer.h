#pragma once

#include "UI/AUIDrawer.h"

namespace TL_UI
{
    class ImGuiUIDrawer :
        public AUIDrawer
    {
    public:
        explicit ImGuiUIDrawer() = default;

        void DrawVector3(AWidgetContainer* _container, const std::string& _name, const TL_Math::Vector3 _value) override;
    };
}
