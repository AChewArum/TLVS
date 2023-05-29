#pragma once

#include <string>

#include "Math/TL_Math.h"

namespace TL_UI
{
    class AWidgetContainer;

    class AUIDrawer abstract
    {
    public:
        virtual void DrawVector3(AWidgetContainer* _container, const std::string& _name, const TL_Math::Vector3 _value) = 0;
    };
}
