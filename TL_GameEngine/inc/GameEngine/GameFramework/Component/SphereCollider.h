#pragma once
#include "GameEngine\GameFramework\Component\Collider.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API SphereCollider :
		public Collider
	{
    public:
        SphereCollider(class GameObject* _gameObject, const tstring& _typeName = TEXT("SphereCollider"));
        virtual ~SphereCollider();

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
        inline TL_Math::Vector3 GetCenter();

        inline void SetRadius(float radius);
        inline float GetRadius();

		// �ʱ�ȭ �ϱ����� �Ӽ����� ��Ʈ���Ϸ��� ����
		inline TL_Physics::SphereColliderInfo GetCapsuleColliderInfo() { return m_SphereColliderInfo; }
		inline void SetSphereColliderInfo(TL_Physics::SphereColliderInfo& info) { m_SphereColliderInfo = info; }
    protected:
        TL_Physics::SphereColliderInfo m_SphereColliderInfo;
	};
}

