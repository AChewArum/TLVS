#include "internal/dll.h"
#include "Common_min.h"
#include "ImGui/ImGuiLayer.h"

namespace TL_UI
{
    class AUserInterface;
}

extern "C"
{
UI_API TL_UI::AUserInterface* CreateUI(TL_Application::ApplicationContext* _appContext, const tstring& _uiType)
{
    TL_UI::AUserInterface* _ui = nullptr;

    if (_uiType == TEXT("ImGui"))
        _ui = new TL_UI::ImGuiLayer;

    _ui->Initialize(_appContext);
    return _ui;
}

UI_API void ReleaseUI(TL_UI::AUserInterface* _ui)
{
    _ui->Release();
    delete _ui;
}
}
