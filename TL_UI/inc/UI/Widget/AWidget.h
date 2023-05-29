#pragma once

#include "UI/RectTransform.h"
#include "UI/API/IDrawable.h"
#include "UI/internal/dll.h"

namespace TL_UI
{
    class UI_API AWidget
        : public IDrawable
    {
    public:
        AWidget() = default;
        virtual ~AWidget() = default;

        void Draw() final;

        RectTransform& GetRect() { return m_Rect; }

    protected:
        virtual void Draw_Impl() = 0;

        RectTransform m_Rect;
    };
}
