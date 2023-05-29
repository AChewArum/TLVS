#pragma once

#include <vector>
#include <memory>

#include "UI/internal/dll.h"
#include "UI/AUIFactory.h"
#include "UI/Widget/AWidget.h"

namespace TL_UI
{
    class AAlternativeRowBackgroundList;

    class UI_API AWidgetContainer abstract
    {
    public:
        AWidgetContainer(AUIFactory* _factory) : m_Factory(_factory) {}
        virtual ~AWidgetContainer() = default;

        AWidgetContainer(const AWidgetContainer& _other) = delete;
        AWidgetContainer(AWidgetContainer&& _other) noexcept = delete;
        AWidgetContainer& operator=(const AWidgetContainer& _other) = delete;
        AWidgetContainer& operator=(AWidgetContainer&& _other) noexcept = delete;

        template <typename TWidget>
        TWidget* CreateWidget()
        {
            auto _widget = m_Factory->CreateWidget<TWidget>();
            StoreAndReturnWidget(_widget);
            return _widget;
        }

    protected:
        AUIFactory* m_Factory;
        std::vector<std::unique_ptr<AWidget>> m_Widgets;

    private:
        AWidget* StoreAndReturnWidget(AWidget* _widget);
    };
}
