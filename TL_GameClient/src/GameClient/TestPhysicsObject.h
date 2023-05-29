#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/GameFramework/Component/IPhysicsEventComponent.h"
#include "GameEngine/Resource/Resource.h"

namespace TL_GameEngine
{
    class Transform;
    class RigidBody;
    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;
    class MeshCollider;
    class GameObject;
    class Scene;
    class StaticMeshRenderer;
    class AudioReverbZones;
}

enum class eColType
{
    BOX
    , SPHERE
    , CAPSULE
    , MESH
};

class TestPhysicsObject
{
public:
    TestPhysicsObject(TL_GameEngine::Scene* scene, float x, float y, float z, bool isRigid, eColType colType);
    ~TestPhysicsObject() = default;

    TL_GameEngine::GameObject* m_pGameObject;
    TL_GameEngine::Transform* m_pTransform;
    TL_GameEngine::RigidBody* m_pRigidBody;
    TL_GameEngine::BoxCollider* m_pBoxCollider;
    TL_GameEngine::SphereCollider* m_pSphereCollider;
    TL_GameEngine::CapsuleCollider* m_pCapsuleCollider;
    TL_GameEngine::MeshCollider* m_pMeshCollider;
};


class TestPlayerComponent :
    public TL_GameEngine::ComponentBase, public TL_GameEngine::ICollisionComponent
{
public:
    TestPlayerComponent(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("TestPlayerComponent"))
        : ComponentBase(_gameObject, _typeName), ICollisionComponent(this) {}

    void OnStart() override;

    void Tick() override;

    void OnCollisionEnter(TL_GameEngine::Collision& collision) override;
    void OnCollisionStay(TL_GameEngine::Collision& collision) override;
    void OnCollisionExit(TL_GameEngine::Collision& collision) override;

    TL_GameEngine::RigidBody* m_pRigidBody;
    TL_GameEngine::AudioReverbZones* m_pReverb;


	std::vector<TL_GameEngine::GameObject*> m_test1;
	std::vector<TL_GameEngine::GameObject*> m_test2;


	TL_GameEngine::Resource<TL_GameEngine::StaticMesh> hahaha;
	TL_Graphics::IMaterial* hahaha2;
	TL_GameEngine::Scene* _scene;
};

namespace TL_Graphics
{
	class IMesh;
	class IMaterial;
	class IConstantBuffer;
	class ICamera;
}

class TestTriggerBoxComponent :
    public TL_GameEngine::ComponentBase, public TL_GameEngine::ITriggerComponent
{
public:
    TestTriggerBoxComponent(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("TestPlayerComponent"))
        : ComponentBase(_gameObject, _typeName), ITriggerComponent(this) {}

    void OnStart() override;

    void Tick() override;

    void OnTriggerEnter(const TL_GameEngine::Collider& other) override;
    void OnTriggerStay(const TL_GameEngine::Collider& other) override;
    void OnTriggerExit(const TL_GameEngine::Collider& other) override;


    TL_GameEngine::Resource<TL_GameEngine::StaticMesh> m_Mesh1;
    TL_GameEngine::Resource<TL_GameEngine::StaticMesh> m_Mesh2;

    TL_GameEngine::StaticMeshRenderer* m_MeshRenderer;

    class TL_GameEngine::BoxCollider* m_BoxCollider;
};

class TestCharactorCtrlComponent :
	public TL_GameEngine::ComponentBase
{
public:
    TestCharactorCtrlComponent(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("TestCharactorCtrlComponent"))
		: ComponentBase(_gameObject, _typeName) {}

	void OnStart() override;

	void Tick() override;

    TL_GameEngine::Transform* m_pCamera = nullptr;
    TL_GameEngine::Transform* m_pControl = nullptr;


    float m_fSpeed = 1.0f;

};

class TestAutoMoveComponent :
	public TL_GameEngine::ComponentBase
{
public:
    TestAutoMoveComponent(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("TestCharactorCtrlComponent"))
		: ComponentBase(_gameObject, _typeName) {}

	void Tick() override;

	float m_fSpeed = 1.0f;

    uint32 m_Dir = 1;

    TL_Math::Vector3 m_Point1;
    TL_Math::Vector3 m_Point2;
};