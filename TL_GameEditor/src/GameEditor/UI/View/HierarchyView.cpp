#include "GameEditor_pch.h"
#include "HierarchyView.h"

#include "GameEditor/EditorApplication.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "UI/AUserInterface.h"
#include "UI/Widget/ATreeView.h"

namespace TL_GameEditor
{
    HierarchyView::HierarchyView():
        m_bOpen(true) { }

    void HierarchyView::Draw()
    {
        if (m_bOpen)
        {
            ImGui::Begin("HierarchyView", &m_bOpen);

            auto _roots = TL_GameEngine::GameApplication::Instance->GetGameWorld()->GetRootGameObjects();

            for (int i = 0; i < _roots.size(); i++)
            {
                DrawNode(_roots[i]);
            }

            ImGui::End();
        }
    }

    void HierarchyView::DrawNode(TL_GameEngine::GameObject* _gameObject)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;

        const std::string _label = StringHelper::ToString(_gameObject->GetName());
        void* const _id = _gameObject;
        const auto _childs = _gameObject->GetChilds();

        bool _bNodeOpen = false;

        if(m_LastSelection == _gameObject)
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (_childs.empty())
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx(_id, node_flags, _label.c_str());
        }
        else
        {
            _bNodeOpen = ImGui::TreeNodeEx(_id, node_flags, _label.c_str());
        }

        if(ImGui::IsItemClicked())
        {
            m_LastSelection = _gameObject;
            EditorApplication::GetInstance()->SetSelectedObject(_gameObject);
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_NODE", &_id, sizeof(void*));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_NODE"))
            {
                // 다른 아이템을 끌어다 이 아이템에 놓았을 때 실행됩니다.

                IM_ASSERT(payload->DataSize == sizeof(void*));
                void* _otherId = *static_cast<void**>(payload->Data);

                OnDrop((TL_GameEngine::GameObject*)_id, (TL_GameEngine::GameObject*)_otherId);
            }
            ImGui::EndDragDropTarget();
        }

        if (_bNodeOpen)
        {
            // 이 노드가 열려있을 때 실행됩니다.
            // 이 노드의 하위 노드들을 그립니다.

            for (int i = 0; i < _childs.size(); i++)
            {
                auto _child = _childs[i];
                DrawNode(_child);
            }

            ImGui::TreePop();
        }
    }

    void HierarchyView::OnDrop(TL_GameEngine::GameObject* _target, TL_GameEngine::GameObject* _other)
    {
        assert(_target != nullptr);
        assert(_other != nullptr);

        _other->SetParent(_target);
    }
}
