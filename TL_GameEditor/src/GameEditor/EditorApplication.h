#pragma once

#include "Common/SingletonInstance.h"

namespace TL_Application
{
    class ApplicationContext;
}

namespace TL_UI
{
    class AUserInterface;
}

namespace TL_GameEngine
{
    class GameObject;
    class GameApplication;
}

namespace TL_GameEditor
{
    class InspectorView;
    class HierarchyView;
    class ImGuiLayer;

    class EditorApplication
    {
        DECLARE_SINGLETON_CLASS(EditorApplication)
        using PROC_CREATE_APPLICATION = TL_GameEngine::GameApplication* (*)(const tstring&);
        using PROC_RELEASE_APPLICATION = void (*)(TL_GameEngine::GameApplication*);

    public:
        void Start(TL_Application::ApplicationContext* _appContext);

        void End();

        void Tick();

    private:
        void CreateGameApplication(const tstring& _param);
        void ReleaseGameApplication();

        void ProcessShortcut();

        void DrawEditorViews();

        TL_Application::ApplicationContext* m_AppContext;
        TL_GameEditor::ImGuiLayer* m_ImGuiLayer;

        HMODULE m_hGameDll;
        TL_GameEngine::GameApplication* m_GameApp;
        PROC_CREATE_APPLICATION m_Proc_CreateGameApplication;
        PROC_RELEASE_APPLICATION m_Proc_ReleaseGameApplication;

        std::unique_ptr<HierarchyView> m_Hierarchy;
        std::unique_ptr<InspectorView> m_Inspector;

        TL_GameEngine::GameObject* m_SelectedObject;

    public:
        TL_Application::ApplicationContext* GetAppContext() const { return m_AppContext; }
        TL_GameEditor::ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

        TL_Application::Window* GetWindow() const;
        TL_Graphics::RenderSystem* GetRenderSystem() const;

        TL_GameEngine::GameObject* GetSelectedObject() const { return m_SelectedObject; }
        void SetSelectedObject(TL_GameEngine::GameObject* const _selectedObject) { m_SelectedObject = _selectedObject; }
    };
}
