#include "GameEngine\GameFramework\Component\Collider.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API CapsuleCollider :
		public Collider
	{
    public:
        CapsuleCollider(class GameObject* _gameObject, const tstring& _typeName = TEXT("CapsuleCollider"));
        ~CapsuleCollider();

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
        inline void SetCenter(float _x, float _y, float _z);
        inline void SetCenter(TL_Math::Vector3 _center);
        inline TL_Math::Vector3 GetCenter();

        inline void SetRadius(float _radius);
        inline float GetRadius();

        inline void SetHeight(float _height);
        inline float GetHeight();

        inline void SetDirection(int _direction);
        inline int GetDirection();

		// 초기화 하기전에 속성들을 컨트롤하려고 만듬
        inline TL_Physics::CapsuleColliderInfo GetCapsuleColliderInfo() { return m_CapsuleColliderInfo; }
        inline void SetCapsuleColliderInfo(TL_Physics::CapsuleColliderInfo& info) { m_CapsuleColliderInfo = info; }

    protected:
       TL_Physics::CapsuleColliderInfo m_CapsuleColliderInfo;
	};
}

