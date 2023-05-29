#pragma once
#include <type_traits>

namespace TL_UI
{
    class APanel;
    class AWidget;
    class AAlternativeRowBackgroundList;
    class ATreeView;

    class AUIFactory abstract
    {
    public:
        AUIFactory() = default;
        virtual ~AUIFactory() = default;

        APanel* CreatePanel();

        template <typename TWidget>
        TWidget* CreateWidget()
        {
            static_assert(std::is_base_of<AWidget, TWidget>::value, "TWidget must inherit from AWidget");
            static_assert(false, "need template specialization for TWidget");
        }

        template <>
        AAlternativeRowBackgroundList* CreateWidget<AAlternativeRowBackgroundList>() { return CreateWidget_Impl_AlternativeRowBackgroundList(); }

        template <>
        ATreeView* CreateWidget<ATreeView>() { return CreateWidget_Impl_ATreeView(); }

    protected:
        virtual APanel* CreatePanel_Impl() = 0;

        virtual AAlternativeRowBackgroundList* CreateWidget_Impl_AlternativeRowBackgroundList() = 0;
        virtual ATreeView* CreateWidget_Impl_ATreeView() = 0;
    };
}
