#pragma once

namespace TL_GameEngine
{
    class GameObject;
}

namespace TL_UI
{
    class APanel;
    class ATreeView;
}

namespace TL_GameEditor
{
    class HierarchyView
    {
    public:
        explicit HierarchyView();

        void Draw();

        void ToggleShow() { m_bOpen = !m_bOpen; }

    private:
        void DrawNode(TL_GameEngine::GameObject* _gameObject);

        void OnDrop(TL_GameEngine::GameObject* _target, TL_GameEngine::GameObject* _other);

    protected:
        bool m_bOpen;
        TL_GameEngine::GameObject* m_LastSelection;
    };
}
