#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/Transform.h"

#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
    Transform::Transform(GameObject* _gameObject, const tstring& _typeName) :
        ComponentBase(_gameObject, _typeName)
        , m_LocalTM(/*Matrix::Identity*/)
        , m_WorldTM(/*Matrix::Identity*/)
        , m_bWorldTMShouldUpdate(false) { }

    ComponentBase* Transform::CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map)
    {
        ComponentBase::CloneInstance_Impl(_cloned, _map);
        Transform* _clonedTransform = static_cast<Transform*>(_cloned);
        _clonedTransform->m_LocalTM = m_LocalTM;
        _clonedTransform->m_WorldTM = m_WorldTM;
        _clonedTransform->m_bWorldTMShouldUpdate = m_bWorldTMShouldUpdate;
        return _cloned;
    }

    void Transform::UpdateWorldMatrix(const Matrix& _parentTM)
    {
        m_WorldTM = GetLocalTM() * _parentTM;

        m_bWorldTMShouldUpdate = false;
    }

	void Transform::ChangeParent(Transform* _parent)
	{
        assert(_parent != nullptr);

        if(_parent == nullptr)
        {
            m_LocalTM = m_WorldTM;
        }
        else
        {
			auto _parentInv = _parent->GetWorldTM().Invert();

			m_LocalTM = m_WorldTM * _parentInv;
        }
	}

	void Transform::SetLocalPosition(const Vector3& _newPosition)
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetLocalTM().Decompose(_scale, _rotation, _position);

        SetLocalTM(
            Matrix::CreateScale(_scale)
            * Matrix::CreateFromQuaternion(_rotation)
            * Matrix::CreateTranslation(_newPosition)
        );
    }

    void Transform::SetLocalRotation(const Quaternion& _newRotation)
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetLocalTM().Decompose(_scale, _rotation, _position);

        SetLocalTM(
            Matrix::CreateScale(_scale)
            * Matrix::CreateFromQuaternion(_newRotation)
            * Matrix::CreateTranslation(_position)
        );
    }

    void Transform::SetLocalScale(const Vector3& _newScale)
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetLocalTM().Decompose(_scale, _rotation, _position);

        SetLocalTM(
            Matrix::CreateScale(_newScale)
            * Matrix::CreateFromQuaternion(_rotation)
            * Matrix::CreateTranslation(_position)
        );
    }

    void TL_GameEngine::Transform::SetLocalPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation)
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetLocalTM().Decompose(_scale, _rotation, _position);

        SetLocalTM(
            Matrix::CreateScale(_scale)
            * Matrix::CreateFromQuaternion(_newRotation)
            * Matrix::CreateTranslation(_newPosition)
        );
    }

    void Transform::SetWorldPosition(const Vector3& _newPosition)
    {
        const auto _parent = GetGameObject()->GetParent();
        if (_parent == nullptr)
        {
            SetLocalPosition(_newPosition);
        }
        else
        {
            auto _newWorldTM = GetWorldTM();

            Vector3 _scale;
            Quaternion _rotation;
            Vector3 _position;
            _newWorldTM.Decompose(_scale, _rotation, _position);

            _newWorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_rotation) * Matrix::CreateTranslation(_newPosition);

            const auto _parentWorldTMInvert = GetGameObject()->GetParent()->GetComponent<Transform>()->GetWorldTM().Invert();
            auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
            SetLocalTM(_newLocalTM);
        }
    }

    void Transform::SetWorldRotation(const Quaternion& _newRotation)
    {
        const auto _parent = GetGameObject()->GetParent();
        if (_parent == nullptr)
        {
            SetLocalRotation(_newRotation);
        }
        else
        {
            auto _newWorldTM = GetWorldTM();

            Vector3 _scale;
            Quaternion _rotation;
            Vector3 _position;
            _newWorldTM.Decompose(_scale, _rotation, _position);

            _newWorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_newRotation) * Matrix::CreateTranslation(_position);

            const auto _parentWorldTMInvert = GetGameObject()->GetParent()->GetComponent<Transform>()->GetWorldTM().Invert();
            auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
            SetLocalTM(_newLocalTM);
        }
    }

    void Transform::SetWorldScale(const Vector3& _newScale)
    {
        const auto _parent = GetGameObject()->GetParent();
        if (_parent == nullptr)
        {
            SetLocalScale(_newScale);
        }
        else
        {
            auto _newWorldTM = GetWorldTM();

            Vector3 _scale;
            Quaternion _rotation;
            Vector3 _position;
            _newWorldTM.Decompose(_scale, _rotation, _position);

            _newWorldTM = Matrix::CreateScale(_newScale) * Matrix::CreateFromQuaternion(_rotation) * Matrix::CreateTranslation(_position);

            const auto _parentWorldTMInvert = GetGameObject()->GetParent()->GetComponent<Transform>()->GetWorldTM().Invert();
            auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
            SetLocalTM(_newLocalTM);
        }
    }



    void TL_GameEngine::Transform::SetPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation)
    {
        const auto _parent = GetGameObject()->GetParent();
        if (_parent == nullptr)
        {
            SetLocalPositionAndRotation(_newPosition, _newRotation);
        }
        else
        {
            Vector3 _scale;
            Quaternion _rotation;
            Vector3 _position;
            m_WorldTM.Decompose(_scale, _rotation, _position);

            m_WorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_newRotation) * Matrix::CreateTranslation(_newPosition);

            SetWorldTM(m_WorldTM);
        }
    }

    Vector3 Transform::GetWorldPosition()
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetWorldTM().Decompose(_scale, _rotation, _position);

        return _position;
    }

    Quaternion Transform::GetWorldRotation()
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetWorldTM().Decompose(_scale, _rotation, _position);
        return _rotation;
    }

    Vector3 Transform::GetWorldScale()
    {
        Vector3 _scale;
        Quaternion _rotation;
        Vector3 _position;
        GetWorldTM().Decompose(_scale, _rotation, _position);

        return _scale;
    }

    void Transform::SetLocalTM(const Matrix& _localTM)
    {
        m_LocalTM = _localTM;
        MarkWorldTMDirt();
    }

    void Transform::SetWorldTM(const Matrix& _worldTM)
    {
        const Matrix _localTM = GetGameObject()->GetParent() == nullptr
            ? _worldTM
            : _worldTM * GetGameObject()->GetParent()->GetComponent<Transform>()->GetWorldTM().Invert();

        SetLocalTM(_localTM);
    }

    Matrix Transform::GetLocalTM() const
    {
        return m_LocalTM;
    }

    Matrix Transform::GetWorldTM()
    {
        TryUpdateWorldTM();

        return m_WorldTM;
    }

    Vector3 Transform::GetRightDirection()
    {
        return GetWorldTM().Right();
    }

    Vector3 Transform::GetUpDirection()
    {
        return GetWorldTM().Up();
    }

    Vector3 Transform::GetForwardDirection()
    {
        return GetWorldTM().Forward();
    }

    void Transform::TryUpdateWorldTM()
    {
        if (m_bWorldTMShouldUpdate == true)
        {
            // 부모의 WorldMatrix를 가져와 새로고칩니다.
            if (m_GameObject->GetParent() == nullptr)
                UpdateWorldMatrix(Matrix::Identity);
            else
                UpdateWorldMatrix(m_GameObject->GetParent()->GetComponent<Transform>()->GetWorldTM());
        }
    }

    void Transform::MarkWorldTMDirt()
    {
        this->m_bWorldTMShouldUpdate = true;

        // 이 게임 오브젝트의 하위 자식 오브젝트들의 TM도 모두 바뀌었기 때문에
        // MarkWorldTMDirt를 전파합니다.
        const auto& _childs = m_GameObject->GetChilds();
        for (auto _child : _childs)
            _child->GetComponent<Transform>()->MarkWorldTMDirt();
    }
}
