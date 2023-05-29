#pragma once

#include "UI/AWidgetContainer.h"
#include "UI/RectTransform.h"
#include "UI/API/IDrawable.h"
#include "UI/internal/dll.h"

namespace TL_UI
{
    class PanelDrawContext
    {
    public:
        float width;
        float height;

        float currentPositionX;
        float currentPositionY;

        void Reset()
        {
            width = 0;
            height = 0;
            currentPositionX = 0;
            currentPositionY = 0;
        }
    };

    class UI_API APanel abstract
        : public AWidgetContainer
          , public IDrawable
    {
    public:
        APanel(AUIFactory* _factory)
            : AWidgetContainer(_factory) {}

        ~APanel() override = default;

        void Draw() override;

        RectTransform& GetRect() { return m_Rect; }
        PanelDrawContext& GetContext() { return m_Context; }

    protected:
        RectTransform m_Rect;
        PanelDrawContext m_Context;
    };
}
