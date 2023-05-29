#include "ImGuiTreeView.h"

#include "Common_min.h"
#include "imgui.h"

namespace TL_UI
{
    void ImGuiTreeView::DrawNodeRecursive_Impl(Target* _data)
    { 
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;

        const std::string _label = GetLabel(_data);
        void* const _id = _data;
        const auto _childs = GetChildList(_data);

        bool _bNodeOpen = false;

        if (_childs.empty())
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx(_id, node_flags, _label.c_str());
        }
        else
        {
            _bNodeOpen = ImGui::TreeNodeEx(_id, node_flags, _label.c_str());
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

                OnDrop(_id, _otherId);
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
                DrawNodeRecursive_Impl(_child);
            }

            ImGui::TreePop();
        }
    }
}
