#include "ImGuiUIFactory.h"

#include "Panel/ImGuiPanel.h"
#include "Widget/ImGuiAlternativeRowBackgroundList.h"
#include "Widget/ImGuiTreeView.h"

namespace TL_UI
{
    APanel* ImGuiUIFactory::CreatePanel_Impl() { return new ImGuiPanel(this); }

    AAlternativeRowBackgroundList* ImGuiUIFactory::CreateWidget_Impl_AlternativeRowBackgroundList() { return new ImGuiAlternativeRowBackgroundList(); }

    ATreeView* ImGuiUIFactory::CreateWidget_Impl_ATreeView() { return new ImGuiTreeView(); }
}
