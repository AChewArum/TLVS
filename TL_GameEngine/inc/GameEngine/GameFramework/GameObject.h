#pragma once

#include "GameWorld.h"
#include "Scene.h"
#include "GameEngine/internal/dll.h"

#include "GameEngine/Object.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "Struct/AddressConvertMap.h"

namespace TL_GameEngine
{
    class ComponentBase;
    class ICollisionComponent;
    class ITriggerComponent;
    class ColliderBase;
    class Scene;

    class GAME_ENGINE_API GameObject final :
        public Object
    {
    public:
        ~GameObject();
        GameObject(const GameObject& _other) = delete;
        GameObject(GameObject&& _other) noexcept = delete;
        GameObject& operator=(const GameObject& _other) = delete;
        GameObject& operator=(GameObject&& _other) noexcept = delete;

        static GameObject* Spawn(
            Scene* _scene,
            const Vector3& _worldPosition = Vector3::Zero,
            const Quaternion& _worldRotation = Quaternion::Identity);

        static GameObject* Spawn(
            Scene* _scene,
            const tstring& _name,
            const Vector3& _worldPosition = Vector3::Zero,
            const Quaternion& _worldRotation = Quaternion::Identity);

        static GameObject* Spawn(
            Scene* _scene,
            GameObject* _cloneTarget,
            const Vector3& _worldPosition = Vector3::Zero,
            const Quaternion& _worldRotation = Quaternion::Identity);

        static GameObject* Create(const tstring& _name = TEXT("New GameObject"));

        void ReserveDestroy();

        void SetScene(Scene* _scene);

        void SetParent(GameObject* _parent);

        /// <summary>
        /// 게임 컴포넌트가 가지고 있는 컴포넌트 중에서 입력받은 타입과 일치하거나 이 타입의 서브타입의 컴포넌트가 있는 경우
        ///	해당 컴포넌트를 반환합니다.
        ///	만약 해당하는 컴포넌트가 없다면 nullptr을 반환합니다.
        /// </summary>
        template <class TComponent>
        TComponent* GetComponent();

        template <class TComponent>
        TComponent* AddComponent();

        template <class TComponent>
        void RemoveComponent();

        template <class TComponent>
        void RemoveComponent(TComponent* _component);

        GameObject* Clone();

        ///*
        //* Physics Event Call back component
        //* Author	: LWT
        //* Date		: 2022. 11. 05
        //*/
        //void OnCollisionEnter(Collision& collision);
        //void OnCollisionStay(Collision& collision);
        //void OnCollisionExit(Collision& collision);
        //void OnTriggerEnter(Collision& collision);
        //void OnTriggerStay(Collision& collision);
        //void OnTriggerExit(Collision& collision);

    private:
        GameObject(const tstring& _typeName = TEXT("GameObject"));

        GameObject* CloneRecursive(AddressConvertMap* _map);

        void CloneConvertAddress(AddressConvertMap* _map);

        bool m_bEnable;
        bool m_bEnableInHierarchy = true;
        GameObjectTag m_Tag;
        Layer m_Layer;

        Scene* m_Scene;
        GameObject* m_Parent;
        std::vector<std::unique_ptr<GameObject>> m_Childs;
        std::vector<std::unique_ptr<ComponentBase>> m_Components;

        std::unique_ptr<GameObject> m_HandleMyself;
        std::vector<ICollisionComponent*> m_CollisionComponents;
        std::vector<ITriggerComponent*> m_TriggerComponents;

    private:
        void AddChild(GameObject* _newChild);

        void RemoveChild(GameObject* _child);

        void SetSceneRecursive(Scene* _scene);

        std::unique_ptr<GameObject> GetUniqueHandleThis();

        std::unique_ptr<GameObject> PopUniqueHandleChild(GameObject* _go);

        void PushUniqueHandleAsChild(std::unique_ptr<GameObject>&& _handle);

        std::unique_ptr<ComponentBase> GetUniqueHandleComponent(ComponentBase* _component);

    public:
        inline bool GetEnable() const { return m_bEnable; }

        inline bool GetActiveInHierarchy() { return m_bEnableInHierarchy; }

        inline class Scene* GetScene() const { return m_Scene; }

        inline GameObject* GetParent() const { return m_Parent; }

        inline GameObjectTag GetTag() const { return m_Tag; }
        inline Layer GetLayer() const { return m_Layer; }

        inline std::vector<GameObject*> GetChilds() const
        {
            std::vector<GameObject*> _out{m_Childs.size()};
            for (int i = 0; i < m_Childs.size(); ++i)
                _out[i] = m_Childs[i].get();
            return _out;
        }

        std::vector<ComponentBase*> GetAllComponents() const
        {
            std::vector<ComponentBase*> _out{m_Components.size()};
            for (int i = 0; i < m_Components.size(); ++i)
                _out[i] = m_Components[i].get();
            return _out;
        }

        inline GameObject* GetChild(size_t _index) const { return m_Childs[_index].get(); }

        GameObject* GetChild(const tstring& _name) const;

        inline void SetEnable(bool _value); // { m_bEnable = _value; }

        inline void SetActiveInHierarchy(bool _value);

        void SetTag(const tstring& _tagName);
        void SetTag(GameObjectTag _tag);

        inline void SetLayer(Layer _value) { m_Layer = _value; }

        void SetLayerRecursive(Layer _value);

        inline GameObject* FindGameObject(const tstring& name);

        inline GameObject* FindChild(const tstring& name) const;

        GameObject* FindChildRecursive(const tstring& name);
        /*
         * 게임 오브젝트가 기본으로 가지고 있는 멤버 컴포넌트를 정의합니다.
         * 컴포넌트 타입의 멤버를 선언하고 GetComponent와 AddComponent 템플릿 특수화 멤버 메소드를 정의합니다.
         */
#define __DEFINE_GAME_OBJECT_GET_EMBEDDED_COMPONENT__(TComponent, ComponentMemberName) \
        public: \
            TComponent ComponentMemberName; \
        template <> \
        TComponent* GetComponent<class TComponent>(){ return &this->ComponentMemberName; } \
        template <> \
        TComponent* AddComponent<class TComponent>() { assert(true); return nullptr; }

        __DEFINE_GAME_OBJECT_GET_EMBEDDED_COMPONENT__(Transform, m_Transform)

        friend class GameWorld;
    };

    template <class TComponent>
    TComponent* GameObject::GetComponent()
    {
        for (const auto& _component : m_Components)
        {
            TComponent* _castedComponent = dynamic_cast<TComponent*>(_component.get());

            if (_castedComponent != nullptr)
                return _castedComponent;
        }

        return nullptr;
    }

    template <class TComponent>
    TComponent* GameObject::AddComponent()
    {
        static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent must inherit from ComponentBase");

        ComponentBase* _newComponent = new TComponent(this);
        m_Components.push_back(std::unique_ptr<ComponentBase>(_newComponent));

        if (m_Scene != nullptr)
        {
            m_Scene->AddComponent(_newComponent);
            GameWorld::Instance->AddComponent(_newComponent);
        }

        return reinterpret_cast<TComponent*>(_newComponent);
    }

    template <class TComponent>
    void GameObject::RemoveComponent()
    {
        for (auto _iter = m_Components.begin();
             _iter != m_Components.end(); ++_iter)
        {
            const auto _castedComponent = dynamic_cast<TComponent*>(_iter->get());
            if (_castedComponent != nullptr)
            {
                RemoveComponent(_castedComponent);

                return;
            }
        }
    }

    template <class TComponent>
    void GameObject::RemoveComponent(TComponent* _component)
    {
        const auto _iter = std::find_if(
            m_Components.begin(), m_Components.end(),
            [_component](const std::unique_ptr<ComponentBase>& _handle) { return _handle.get() == _component; });
        assert(_iter != m_Components.end());

        // 컴포넌트 인스턴스 소멸은 GameWorld에서 실행합니다.
        // 왜냐하면, 다음 프레임까지 삭제를 지연시킨 뒤
        // 파괴 예약된 컴포넌트들을 한 번에 소멸시켜야 하기 때문입니다.

        // 컴포넌트를 world에서 삭제합니다.
        GameWorld::GetInstance()->ReserveDestroy(_component);

        // 충돌체 컴포넌트라면, 충돌체 컴포넌트 목록에서 제외합니다.

        if (_component->CheckFlag(ComponentFlag::eTRIGGER))
        {
            auto _triggerIter = std::find(
                m_TriggerComponents.begin(), m_TriggerComponents.end(),
                reinterpret_cast<ITriggerComponent*>(_component)
            );
            assert(_triggerIter != m_TriggerComponents.end());
            m_TriggerComponents.erase(_triggerIter);
        }
        if (_component->CheckFlag(ComponentFlag::eCOLLISION))
        {
            auto _collisionIter = std::find(
                m_CollisionComponents.begin(), m_CollisionComponents.end(),
                reinterpret_cast<ICollisionComponent*>(_component)
            );
            assert(_collisionIter != m_CollisionComponents.end());
            m_CollisionComponents.erase(_collisionIter);
        }
    }
}
