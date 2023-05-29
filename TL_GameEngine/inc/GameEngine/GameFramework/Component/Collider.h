#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Physics/PhysicsInfo.h"

//#include "Physics/Metal/PhysicsMaterial.h"
//#include "Physics/PhysicsObject.h"

namespace TL_Physics
{
    class PhysicsMaterial;
    class PhysicsObject;
}

namespace TL_GameEngine
{
    class RigidBody;
    class Transform;

    class GAME_ENGINE_API Collider :
        public ComponentBase
    {
    public:
        Collider(class GameObject* _gameObject, const tstring& _typeName, TL_Physics::ColliderInfo&);
        virtual ~Collider();

        //////////////////////////////////////////////////////////////////////////
        // Inherited Component
        //////////////////////////////////////////////////////////////////////////

        virtual void FixedTick();

        virtual void PreTick();

        virtual void PostTick();

        virtual void TryTick();

        virtual void Tick();

        virtual void OnStart();

        virtual void OnChangeEnable(bool _newEnable);

        //////////////////////////////////////////////////////////////////////////
        // Member Function
        //////////////////////////////////////////////////////////////////////////
 
        bool IsDirty() { return m_IsDirty; }

        void AttachRigidBody(RigidBody* _rigidBody);

        void DetachRigidBody();

        void FetchResult();

        /// <summary>
        /// 뭐 어떻게 할건지 몰라서 임시로 해둠
        /// </summary>

        void OnCollisionEnterMessage(class Collision& collision);
        void OnCollisionStayMessage(class Collision& collision);
        void OnCollisionExitMessage(class Collision& collision);
        void OnTriggerEnterMessage(class Collider& collider);
        void OnTriggerStayMessage();
        void OnTriggerExitMessage(class Collider& collider);

        //void TriggerStayCallback();

        //////////////////////////////////////////////////////////////////////////
        // Getter Setter
        //////////////////////////////////////////////////////////////////////////

        inline RigidBody* GetAttachRigidBody();

        inline UID GetMaterialUID();

        inline void SetMaterial(TL_Physics::PhysicsMaterial* _material);
        inline TL_Physics::PhysicsMaterial* GetMaterial();

        inline void SetTrigger(bool flag);
        inline bool GetTrigger(); 

    protected:
        bool m_IsDirty = true;

        bool m_IsAttachedRigidBody = false;
        RigidBody* m_pAttachedRigidBody = nullptr;

        // Todo : GameObject나 componentbase 에 short cut이 생기면 대체해야함
        Transform* m_pTransform = nullptr;
        TL_Math::Vector3 m_PrevScale;


        bool m_IsTrigger = false;

        UID m_PhysicsMaterialUID;
        TL_Physics::PhysicsMaterial* m_pPhysicsMaterial = nullptr; // Todo : 고민중 어떻게 해야 할까

        TL_Physics::PhysicsObject* m_pPhysicsObject = nullptr;

        TL_Physics::ColliderInfo& m_ColliderInfoRef;
    private:
        std::vector<class ICollisionComponent*> m_CollisionComponents;
        std::vector<class ITriggerComponent*> m_TriggerComponents;

        std::vector<Collider*> m_OtherColliders;
    };
}

