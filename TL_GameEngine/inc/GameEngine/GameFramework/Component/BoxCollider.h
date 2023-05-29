#pragma once

#include "GameEngine\GameFramework\Component\Collider.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API BoxCollider : public Collider
	{
    public:
        BoxCollider(class GameObject* _gameObject, const tstring& _typeName = TEXT("BoxCollider"));
        virtual ~BoxCollider();

        //////////////////////////////////////////////////////////////////////////
        // Inherited Component
        //////////////////////////////////////////////////////////////////////////

        virtual void PreTick();

        virtual void PostTick();

        virtual void TryTick();

        virtual void Tick();

        virtual void OnStart();

        virtual void OnChangeEnable(bool _newEnable);

        //////////////////////////////////////////////////////////////////////////
        // Getter & Setter
        //////////////////////////////////////////////////////////////////////////

        inline void SetCenter(float x, float y, float z);
        inline void SetCenter(TL_Math::Vector3 center);
        inline TL_Math::Vector3& GetCenter();

        inline void SetSize(float x, float y, float z);
        inline void SetSize(TL_Math::Vector3 size);
        inline TL_Math::Vector3& GetSize();

		// 초기화 하기전에 속성들을 컨트롤하려고 만듬
		inline TL_Physics::BoxColliderInfo GetBoxColliderInfo() { return m_BoxColliderInfo; }
		inline void SetBoxColliderInfo(TL_Physics::BoxColliderInfo& info) { m_BoxColliderInfo = info; }

	protected:
        TL_Physics::BoxColliderInfo m_BoxColliderInfo;
	};
}