#pragma once

//#include "GameEngine/internal/dll.h"

#include "GameEngine/GameFramework/Component/ComponentBase.h"

#include "Common.h"
#include "GameEngine/Object.h"

namespace TL_GameEngine
{
    class Collider;
    class Collision;

    class GAME_ENGINE_API ICollisionComponent abstract
    {
    public:
        ICollisionComponent(ComponentBase* _componentBase)
        {
            _componentBase->AddFlag(ComponentFlag::eCOLLISION);
        }
        virtual void OnCollisionEnter(Collision& _collision) {};
        virtual void OnCollisionStay(Collision& _collision) {};
        virtual void OnCollisionExit(Collision& _collision) {};

    };

    class GAME_ENGINE_API ITriggerComponent abstract
    {
    public:
        ITriggerComponent(ComponentBase* _componentBase)
        {
            _componentBase->AddFlag(ComponentFlag::eTRIGGER);
        }
        virtual void OnTriggerEnter(const Collider& _other) {};
        virtual void OnTriggerStay(const Collider& _other) {};
        virtual void OnTriggerExit(const Collider& _other) {};
    };


    class GAME_ENGINE_API TriggerComponentBase : public ComponentBase
    {
    public:
        TriggerComponentBase(GameObject* _gameObject, const tstring& _typeName);

        virtual ~TriggerComponentBase() = default;

        virtual void PreTick() {}

        virtual void PostTick() {}

        virtual void TryTick() {}

        virtual void Tick() {}

        virtual void OnStart();

        virtual void OnChangeEnable(bool _newEnable);

        virtual void OnTriggerEnter(const Collider& _other) abstract;
        virtual void OnTriggerStay(const Collider& _other) abstract;
        virtual void OnTriggerExit(const Collider& _other) abstract;

    private:
        bool m_bIsPrevEnter = false;
    };

    class GAME_ENGINE_API CollisionComponentBase : public ComponentBase
    {
    public:
        CollisionComponentBase(GameObject* _gameObject, const tstring& _typeName);

        virtual ~CollisionComponentBase() = default;

        virtual void PreTick() {}

        virtual void PostTick() {}

        virtual void TryTick() {}

        virtual void Tick() {}

        virtual void OnStart() {}

        virtual void OnChangeEnable(bool _newEnable) {}

        virtual void OnCollisionEnter(Collision& _collision) abstract;
        virtual void OnCollisionStay(Collision& _collision) abstract;
        virtual void OnCollisionExit(Collision& _collision) abstract;

        bool m_bIsPrevEnter = false;
    };
}
