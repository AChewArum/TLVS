#pragma once
#include "GameEngine\Gameframework\Component\Collider.h"

#include "GameEngine\Resource\Resource.h"

namespace TL_GameEngine
{
    class StaticMesh;

    class GAME_ENGINE_API MeshCollider :
        public Collider
    {
    public:
        MeshCollider(class GameObject* _gameObject, const tstring& _typeName = TEXT("MeshCollider"));
        virtual ~MeshCollider();

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

        inline void SetConvex(bool flag);
        inline bool GetConvex();

        inline void SetIsTrigger(bool flag);
        inline bool GetIsTrigger(bool flag);

        inline void SetMesh(Resource<StaticMesh> meshRes);
        inline Resource<StaticMesh> GetMesh();

        // 초기화 하기전에 속성들을 컨트롤하려고 만듬
		inline TL_Physics::MeshColliderInfo GetMeshColliderInfo() { return m_MeshColliderInfo; }
		inline void SetMeshColliderInfo(TL_Physics::MeshColliderInfo& info) { m_MeshColliderInfo = info; }

    protected:
        TL_Physics::MeshColliderInfo m_MeshColliderInfo;

        std::vector<float> m_MeshData;

        Resource<StaticMesh> m_pMesh;
    };
}
