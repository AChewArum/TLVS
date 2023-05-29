#pragma once
#include "UI/AUIFactory.h"

namespace TL_UI
{
    class AUserInterface;

    class ImGuiUIFactory
        : public AUIFactory
    {
    public:
        ImGuiUIFactory() = default;
        ~ImGuiUIFactory() override = default;

    protected:
        APanel* CreatePanel_Impl() override;

        AAlternativeRowBackgroundList* CreateWidget_Impl_AlternativeRowBackgroundList() override;
        ATreeView* CreateWidget_Impl_ATreeView() override;
    };
}
