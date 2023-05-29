#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/GameObject.h"

#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/GameFramework/Component/IPhysicsEventComponent.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "Log/Log.h"

namespace TL_GameEngine
{
    GameObject::~GameObject()
    {
        // release는 현재 소유한 인스턴스의 소유권을 해제 없이 놓는 함수입니다.
        // 이 게임 오브젝트가 월드에 속하지 않아 HandleMyself에 자기 자신의 포인터가 저장되어 있는 경우
        // 재귀적으로 삭제하려 하는 것을 막기 위해 이를 수행해야 합니다.
        m_HandleMyself.release();

        if (m_Scene != nullptr)
        {
            for (const auto& c : m_Components)
            {
                GameWorld::Instance->RemoveComponent(c.get());
                m_Scene->RemoveComponent(c.get());
            }

            if (m_Parent == nullptr)
            {
                m_Scene->RemoveRootGameObject(this);
            }
        }
    }

    GameObject* GameObject::Spawn(
        Scene* _scene,
        const Vector3& _worldPosition,
        const Quaternion& _worldRotation
    )
    {
        return Spawn(_scene, TEXT("New GameObject"), _worldPosition, _worldRotation);
    }

    GameObject* GameObject::Spawn(
        Scene* _scene,
        const tstring& _name,
        const Vector3& _worldPosition,
        const Quaternion& _worldRotation
    )
    {
        GameObject* _go = new GameObject();

        _go->SetName(_name);
        _go->SetScene(_scene);
        _go->m_Transform.SetWorldPosition(_worldPosition);
        _go->m_Transform.SetWorldRotation(_worldRotation);

        return _go;
    }

    GameObject* GameObject::Spawn(
        Scene* _scene,
        GameObject* _cloneTarget,
        const Vector3& _worldPosition,
        const Quaternion& _worldRotation
    )
    {
        GameObject* _cloned = _cloneTarget->Clone();

        _cloned->SetScene(_scene);
        _cloned->m_Transform.SetWorldPosition(_worldPosition);
        _cloned->m_Transform.SetWorldRotation(_worldRotation);

        return _cloned;
    }

    GameObject* GameObject::Create(const tstring& _name)
    {
        GameObject* _go = new GameObject();
        _go->m_Name = _name;

        return _go;
    }

    void GameObject::ReserveDestroy()
    {
        GameWorld::GetInstance()->ReserveDestroy(this);
    }

    void GameObject::SetScene(Scene* _scene)
    {
        if (m_Scene == _scene)
            return;

        std::unique_ptr<GameObject> _handleThis = GetUniqueHandleThis();

        if (_scene != nullptr)
        {
            // 이제 이동 대상 씬에서 루트 오브젝트가 되므로
            // 루트 오브젝트 목록에 추가합니다.
            GameWorld::GetInstance()->PushUniqueHandleAsRoot(std::move(_handleThis));
            _scene->AddRootGameObject(this);
        }

        if (m_Parent == nullptr && m_Scene != nullptr)
        {
            // 원래 포함되어 있던 씬에서 루트 오브젝트였던 경우,
            // 원래 씬의 루트 오브젝트 목록에서 제외합니다.
            m_Scene->RemoveRootGameObject(this);
        }

        m_Parent = nullptr;

        SetSceneRecursive(_scene);
    }

    void GameObject::SetParent(GameObject* _parent)
    {
        // 기존의 부모와 같다면, 아무 처리도 하지 않습니다.
        if (m_Parent == _parent)
            return;

        const Matrix _worldTMOrigin = GetComponent<Transform>()->GetWorldTM();
        Scene* const _targetScene = _parent != nullptr ? _parent->GetScene() : m_Scene;
        const bool _bSceneChanged = m_Scene != _targetScene;

        std::unique_ptr<GameObject> _handleThis = GetUniqueHandleThis();

        if (m_Scene != nullptr)
        {
            if (m_Parent == nullptr)
            {
                // m_Parent == nullptr && _parent != nullptr

                // 원래 부모를 가지고 있지 않았던 경우,
                // 루트였기 때문에 씬의 루트 목록에서 제외합니다.
                m_Scene->RemoveRootGameObject(this);
            }

            if (_parent == nullptr)
            {
                // m_Parent != nullptr && _parent == nullptr

                // 원래 부모를 가지고 있던 경우,
                // 루트가 아니였기 때문에 씬에 새로이 루트로 등록합니다.
                _targetScene->AddRootGameObject(this);
            }
        }

        if (_parent != nullptr)
        {
            _parent->PushUniqueHandleAsChild(std::move(_handleThis));
        }
        else if (_targetScene != nullptr)
        {
            // parent == nullptr && targetScene != nullptr

            // 게임 월드 내 오브젝트였다면, 월드에 루트로 등록합니다.
            GameWorld::GetInstance()->PushUniqueHandleAsRoot(std::move(_handleThis));
        }
        else
        {
            // parent == nullptr && targetScene != nullptr

            this->m_HandleMyself = std::move(_handleThis);
        }

        m_Parent = _parent;
        GetComponent<Transform>()->SetWorldTM(_worldTMOrigin);

        if (m_Scene != nullptr)
        {
            SetSceneRecursive(m_Scene);
        }
    }

    GameObject* GameObject::Clone()
    {
        const auto _map = std::make_unique<AddressConvertMap>();
        auto _cloned = CloneRecursive(_map.get());
        _cloned->CloneConvertAddress(_map.get());
        _cloned->SetName(_cloned->GetName() + TEXT("_Clone"));
        return _cloned;
    }

    GameObject::GameObject(const tstring& _typeName) :
        Object(_typeName),
        m_bEnable(true),
        m_Tag(0),
        m_Layer(0),
        m_Scene(nullptr),
        m_Parent(nullptr),
        m_Childs(0),
        m_Components(0),
        m_HandleMyself(this),
        m_Transform(Transform(this)) { }

    GameObject* GameObject::CloneRecursive(AddressConvertMap* _map)
    {
        GameObject* _cloned = new GameObject();
        _map->Mapping(this, _cloned);

        Log::Engine_Debug("clone {0}: {1} -> {2}",
                          StringHelper::ToString(m_Name), (void*)this, (void*)_cloned);

        _cloned->m_Name = m_Name;
        _cloned->m_bEnable = m_bEnable;
        _cloned->m_Layer = m_Layer;
        // _cloned->m_Parent = m_Parent;
        // _cloned->m_Childs = m_Childs;
        // _cloned->m_Components = m_Components;

        m_Transform.CloneInstance_Impl(&_cloned->m_Transform, _map);

        for (const auto& _child : m_Childs)
        {
            auto _newChild = _child->CloneRecursive(_map);
            _newChild->SetParent(_cloned);
        }

        for (const auto& _component : m_Components)
        {
            ComponentBase* _newComp = _component->Clone_WithoutConvertAddress(_cloned, _map);
            _cloned->m_Components.push_back(std::unique_ptr<ComponentBase>(_newComp));
        }

        return _cloned;
    }

    void GameObject::CloneConvertAddress(AddressConvertMap* _map)
    {
        m_Transform.CloneConvertAddress_Impl(_map);

        for (const auto& _component : m_Components)
        {
            _component->CloneConvertAddress_Impl(_map);
        }

        for (const auto& _child : m_Childs)
        {
            _child->CloneConvertAddress(_map);
        }
    }

    void GameObject::AddChild(GameObject* _newChild)
    {
        const auto _iter = std::find_if(
            m_Childs.begin(), m_Childs.end(),
            [_newChild](const std::unique_ptr<GameObject>& _handle) { return _handle.get() == _newChild; });
        assert(_iter == m_Childs.end());

        m_Childs.push_back(_newChild->GetUniqueHandleThis());
    }

    void GameObject::RemoveChild(GameObject* _child)
    {
        const auto _iter = std::find_if(
            m_Childs.begin(), m_Childs.end(),
            [_child](const std::unique_ptr<GameObject>& _handle) { return _handle.get() == _child; });
        assert(_iter != m_Childs.end());
        m_Childs.erase(_iter);
    }

    void GameObject::SetSceneRecursive(Scene* _scene)
    {
        if (m_Scene != nullptr)
        {
            std::for_each(m_Components.begin(), m_Components.end(),
                          [this](const std::unique_ptr<ComponentBase>& comp) { m_Scene->RemoveComponent(comp.get()); });
        }

        if (_scene != nullptr)
        {
            std::for_each(m_Components.begin(), m_Components.end(),
                          [_scene](const std::unique_ptr<ComponentBase>& comp) { _scene->AddComponent(comp.get()); });
        }

        if (m_Scene == nullptr && _scene != nullptr)
        {
            std::for_each(m_Components.begin(), m_Components.end(),
                          [](const std::unique_ptr<ComponentBase>& comp) { GameWorld::Instance->AddComponent(comp.get()); });
        }
        else if (m_Scene != nullptr && _scene == nullptr)
        {
            std::for_each(m_Components.begin(), m_Components.end(),
                          [](const std::unique_ptr<ComponentBase>& comp) { GameWorld::Instance->RemoveComponent(comp.get()); });
        }

        m_Scene = _scene;

        for (auto _child : GetChilds())
        {
            // 자식을 가진 게임 오브젝트를 이동시키는 경우,
            // 자식 오브젝트들 모두 전환 대상이 되는 씬에 포함시킵니다.

            _child->SetSceneRecursive(_scene);
        }
    }

    std::unique_ptr<GameObject> GameObject::GetUniqueHandleThis()
    {
        if (m_HandleMyself.get() != nullptr)
        {
            return std::move(m_HandleMyself);
        }
        else if (m_Parent != nullptr)
        {
            return std::move(m_Parent->PopUniqueHandleChild(this));
        }
        else
        {
            return std::move(GameWorld::GetInstance()->PopUniqueHandleOf(this));
        }
    }

    std::unique_ptr<GameObject> GameObject::PopUniqueHandleChild(GameObject* _go)
    {
        auto _iter = std::find_if(
            m_Childs.begin(), m_Childs.end(),
            [_go](const std::unique_ptr<GameObject>& _handle) { return _handle.get() == _go; });
        assert(_iter != m_Childs.end());

        std::unique_ptr<GameObject> _outHandle = std::move(*_iter);
        m_Childs.erase(_iter);
        return std::move(_outHandle);
    }

    void GameObject::PushUniqueHandleAsChild(std::unique_ptr<GameObject>&& _handle)
    {
        auto _iter = std::find(m_Childs.begin(), m_Childs.end(), _handle);
        assert(_iter == m_Childs.end());

        m_Childs.push_back(std::move(_handle));
    }

    std::unique_ptr<ComponentBase> GameObject::GetUniqueHandleComponent(ComponentBase* _component)
    {
        auto _iter = std::find_if(
            m_Components.begin(), m_Components.end(),
            [_component](const std::unique_ptr<ComponentBase>& _handle) { return _handle.get() == _component; });
        assert(_iter != m_Components.end());

        std::unique_ptr<ComponentBase> _outHandle = std::move(*_iter);
        m_Components.erase(_iter);
        return std::move(_outHandle);
    }

    GameObject* GameObject::GetChild(const tstring& _name) const
    {
        for (const auto& _child : m_Childs)
        {
            if (_child->GetName() == _name)
                return _child.get();
        }

        return nullptr;
    }

    void GameObject::SetEnable(bool _value)
    {
        m_bEnable = _value;

        if (m_bEnableInHierarchy)
        {
            m_bEnableInHierarchy = _value;

            for (auto& _gameObjectIter : m_Childs)
            {
                _gameObjectIter->SetActiveInHierarchy(_value);
            }
        }
    }

    void GameObject::SetActiveInHierarchy(bool _value)
    {
        if (m_bEnableInHierarchy != _value)
        {
            m_bEnableInHierarchy = _value;

            for (auto& _gameObjectIter : m_Childs)
            {
                if (m_bEnable == true)
                {
                    _gameObjectIter->SetActiveInHierarchy(_value);
                }
            }
        }
    }

    void GameObject::SetTag(const tstring& _tagName)
    {
        SetTag(GameWorld::GetInstance()->FindGameObjectTag(_tagName));
    }

    void GameObject::SetTag(GameObjectTag _tag)
    {
        if (m_Tag == _tag)
            return;

        m_Tag = _tag;
    }

    void GameObject::SetLayerRecursive(Layer _value)
    {
        m_Layer = _value;
        for (auto _child : GetChilds())
        {
            //_child가 _go로 되어있어서 바꿈
            _child->SetLayer(_value);
            _child->SetLayerRecursive(_value);
        }
    }

    GameObject* GameObject::FindGameObject(const tstring& name)
    {
        GameObject* _findGameObject = nullptr;

        if (GetName() == name)
        {
            return this;
        }

        for (const auto& _child : m_Childs)
        {
            _findGameObject = _child->FindGameObject(name);

            if (_findGameObject != nullptr)
            {
                return _findGameObject;
            }
        }

        return _findGameObject;
    }

    GameObject* GameObject::FindChild(const tstring& name) const
    {
        for (const auto& _child : m_Childs)
        {
            if (_child->GetName() == name)
            {
                return _child.get();
            }
        }

        return nullptr;
    }

    GameObject* GameObject::FindChildRecursive(const tstring& name)
    {
        if (GetName() == name)
        {
            return this;
        }

        for (const auto& _child : m_Childs)
        {
            GameObject* _findGameObject = _child->FindGameObject(name);

            if (_findGameObject != nullptr)
                return _findGameObject;
        }
    }
}
