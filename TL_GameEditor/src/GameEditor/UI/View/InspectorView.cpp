#include "GameEditor_pch.h"
#include "InspectorView.h"

#include "GameEditor/EditorApplication.h"
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEditor
{
    InspectorView::InspectorView()
        : m_bOpen(true) {}

    void InspectorView::Draw()
    {
        if (m_bOpen)
        {
            ImGui::Begin("InspectorView", &m_bOpen);

            TL_GameEngine::GameObject* const _selected = EditorApplication::GetInstance()->GetSelectedObject();
            if (_selected != nullptr)
            {
                std::string _name = StringHelper::ToString(_selected->GetName());
                TL_Math::Vector3 _position = _selected->m_Transform.GetLocalPosition();
                TL_Math::Vector3 _rotationDeg = _selected->m_Transform.GetLocalRotation().ToEulerDeg();
                TL_Math::Vector3 _scale = _selected->m_Transform.GetLocalScale();

                bool _bChanged = false;

                // 이름을 표시합니다.

                static char _nameBuffer[256] = "";
                static char _emptyNameBuffer[256] = "";
                std::copy(&_emptyNameBuffer[0], &_emptyNameBuffer[255], _nameBuffer);
                std::copy(_name.begin(), _name.end(), _nameBuffer);

                _bChanged = ImGui::InputText("Name", _nameBuffer, 256);
                if (_bChanged)
                {
                    tstring _newName = StringHelper::ToTString(std::string(_nameBuffer));
                    _selected->SetName(_newName);
                }

                // local transform을 표시합니다.

                _bChanged = ImGui::DragFloat3("Position", (float*)&_position);
                if (_bChanged)
                {
                    _selected->m_Transform.SetLocalPosition(_position);
                }

                _bChanged = ImGui::DragFloat3("Rotation", (float*)&_rotationDeg);
                if (_bChanged)
                {
                    _selected->m_Transform.SetLocalRotation(TL_Math::Quaternion::CreateFromYawPitchRollDeg(_rotationDeg));
                }

                _bChanged = ImGui::DragFloat3("Scale", (float*)&_scale);
                if (_bChanged)
                {
                    _selected->m_Transform.SetLocalScale(_scale);
                }
            }

            ImGui::End();
        }
    }
}
