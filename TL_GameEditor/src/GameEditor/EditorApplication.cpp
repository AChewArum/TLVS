#include "GameEditor_pch.h"
#include "GameEditor/EditorApplication.h"

#include "Application/Input.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "UI/AUserInterface.h"
#include "UI/ImGuiLayer.h"
#include "UI/View/HierarchyView.h"
#include "UI/View/InspectorView.h"

using namespace TL_Application;

namespace TL_GameEditor
{
    DEFINE_SINGLETON_CLASS(
        EditorApplication,
        []() {},
        []() {}
    )

    void EditorApplication::Start(TL_Application::ApplicationContext* _appContext)
    {
        m_AppContext = _appContext;
        m_ImGuiLayer = new ImGuiLayer();
        m_ImGuiLayer->Initialize(_appContext);
        m_Hierarchy = std::move(std::make_unique<HierarchyView>());
        m_Inspector = std::move(std::make_unique<InspectorView>());

        // Test
        CreateGameApplication(StringHelper::ToTString(_appContext->GetCommandLineArgs().find("app")->second));
    }

    void EditorApplication::End()
    {
        if (m_GameApp != nullptr)
            ReleaseGameApplication();

        m_ImGuiLayer->Release();
        m_ImGuiLayer = nullptr;
    }

    void EditorApplication::Tick()
    {
        m_AppContext->PreTick();
        m_ImGuiLayer->PreRender();

        ProcessShortcut();

        if (m_GameApp != nullptr)
        {
            m_GameApp->Tick();
        }

        // Objects를 그립니다.

        m_AppContext->PreRenderObjects();
        if (m_GameApp != nullptr)
        {
            // Editor 내에서 GameApplication을 실행중이라면,
            // PreRenderObjects와 PostRenderObjects 사이에서 RenderTick을 실행합니다.
            m_GameApp->RenderTick();
        }
        m_AppContext->PostRenderObjects();

        // Editor UI를 그립니다.

        DrawEditorViews();
        m_ImGuiLayer->PostRender();

        m_AppContext->Present();
    }

    void EditorApplication::CreateGameApplication(const tstring& _param)
    {
        assert(m_hGameDll == nullptr);
        assert(m_GameApp == nullptr);

        HMODULE _hDLL = LoadLibrary(TEXT("./GameClient.dll"));
        assert(_hDLL != nullptr);

        m_Proc_CreateGameApplication = (PROC_CREATE_APPLICATION)GetProcAddress(_hDLL, "CreateApplication");
        m_Proc_ReleaseGameApplication = (PROC_RELEASE_APPLICATION)GetProcAddress(_hDLL, "ReleaseApplication");
        assert(m_Proc_CreateGameApplication != nullptr);
        assert(m_Proc_ReleaseGameApplication != nullptr);

        m_GameApp = m_Proc_CreateGameApplication(_param);
        assert(m_GameApp != nullptr);

        m_hGameDll = _hDLL;

        m_GameApp->Start(m_AppContext);
    }

    void EditorApplication::ReleaseGameApplication()
    {
        assert(m_hGameDll != nullptr);
        assert(m_GameApp != nullptr);

        m_GameApp->End();

        m_Proc_ReleaseGameApplication(m_GameApp);
        m_GameApp = nullptr;

        FreeLibrary(m_hGameDll);
        m_hGameDll = nullptr;
    }

    void EditorApplication::ProcessShortcut()
    {
        if (Input::GetKeyPress(EVirtualKey::Control) && Input::GetKeyDown(EVirtualKey::Key_3))
        {
            m_Inspector->ToggleShow();
        }

        if (Input::GetKeyPress(EVirtualKey::Control) && Input::GetKeyDown(EVirtualKey::Key_4))
        {
            m_Hierarchy->ToggleShow();
        }

        if (Input::GetKeyDown(EVirtualKey::Delete))
        {
            if (m_SelectedObject != nullptr)
            {
                m_SelectedObject->ReserveDestroy();
                m_SelectedObject = nullptr;
            }
        }
    }

    void EditorApplication::DrawEditorViews()
    {
        m_Hierarchy->Draw();
        m_Inspector->Draw();
    }

    TL_Application::Window* EditorApplication::GetWindow() const { return m_AppContext->GetWindow(); }
    TL_Graphics::RenderSystem* EditorApplication::GetRenderSystem() const { return m_AppContext->GetRenderSystem(); }
}
