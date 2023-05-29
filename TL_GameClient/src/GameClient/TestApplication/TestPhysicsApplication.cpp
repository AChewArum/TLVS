#include "GameClient/TestApplication/TestPhysicsApplication.h"

#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Scene.h"

#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/GameFramework/Component/BoxCollider.h"
#include "GameEngine/GameFramework/Component/RigidBody.h"
#include "GameEngine/GameFramework/Component/MeshCollider.h"

#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/AudioReverbZones.h"
#include "GameEngine/GameFramework/Sound/AudioSystem.h"
#include "GameEngine/GameFramework/Sound/AudioClip.h"

#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/GameFramework/Component/FreeLookCamera.h"

#include "Graphics/RenderSystem.h"

#include "GameClient/TestPhysicsObject.h"
#include "GameEngine/GameFramework/Component/Camera.h"

using namespace TL_GameEngine;

void TestPhysicsApplication::OnApplicationStart()
{
    GetRenderSystem()->UpdateWindowSize(1280, 720);

    GameApplication::OnApplicationStart();

    _scene = new Scene(TEXT("PhysicsScene"));

    struct Vertex
    {
        float position[3];
        float color[3];
    }
        _vertices3[8]
        {
            {{+0.033264f, +0.887192f, -0.003035f}, {1.0f, 0.0f, 1.0f}},
            {{+1.16321f, -0.463653f, +0.049498f}, {1.0f, 0.0f, 1.0f}},
            {{-0.64378f, -0.463653f, +1.14096f}, {1.0f, 0.0f, 1.0f}},
            {{-0.642243f, -0.463653f, -1.03863f}, {1.0f, 0.0f, 1.0f}}
        };

    TL_Graphics::VertexAttribute _vertexAttribute3;
    _vertexAttribute3.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
    _vertexAttribute3.AddElementToDesc(sizeof(Vertex::color), TL_Graphics::DataType::FLOAT, "COLOR");
    _vertexAttribute3.AddData(_vertices3, sizeof(_vertices3));

    std::vector<TL_Common::StaticVertex> _staticVertices3;

    _staticVertices3.resize(4);
    _staticVertices3[0].position = {+0.033264f, +0.887192f, -0.003035f}; //, { 1.0f, 0.0f, 1.0f }},
    _staticVertices3[1].position = {+1.16321f, -0.463653f, +0.049498f}; // , { 1.0f, 0.0f, 1.0f }},
    _staticVertices3[2].position = {-0.64378f, -0.463653f, +1.14096f}; // , { 1.0f, 0.0f, 1.0f }},
    _staticVertices3[3].position = {-0.642243f, -0.463653f, -1.03863f};

    std::vector<UINT> indicies2
        =
        {
            0, 2, 1,
            0, 3, 2,
            0, 1, 3,
            1, 2, 3
        };

    auto _graphicsMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(_vertexAttribute3, indicies2.data(), 12, TEXT("Shader/ColorMeshVS.hlsl"));
    const auto _mesh = new StaticMesh(_staticVertices3, indicies2, std::vector({std::pair<uint, int>(0, 0)}), _graphicsMesh);

    Resource<StaticMesh> _meshRes;
    _meshRes.SetPtr(_mesh);

    const auto _material = new Material();
    _material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial();
    m_pCameraObject = GameObject::Spawn(_scene);
    m_pCameraObject->AddComponent<Camera>();
    //m_pCameraObject->SetParent(_pTestObject1->m_pGameObject);

    Resource<Material> _materialRes;
    _materialRes.SetPtr(_material);

    m_pMeshObject = GameObject::Spawn(_scene);
    auto* _goRen = m_pMeshObject->AddComponent<StaticMeshRenderer>();
    auto* _goMeshCol = m_pMeshObject->AddComponent<MeshCollider>();
    //_goMeshCol->SetConvex(true);
    _goMeshCol->SetMesh(_meshRes);

    _goRen->SetMesh(_meshRes);
    _goRen->SetMaterial({_materialRes});
    m_pMeshObject->AddComponent<RigidBody>();

    GameWorld::GetInstance()->ReserveLoadScene(_scene);
}

void TestPhysicsApplication::OnApplicationTick()
{
    GameApplication::OnApplicationTick();
}

void TestPhysicsApplication::OnApplicationEnd()
{
    GameApplication::OnApplicationEnd();

    //delete m_pMeshObject;
    //delete m_pCameraObject;
    TL_Graphics::RenderSystem::Get()->Return(m_Material);

    //delete _pTestObject1;
    //delete _pTestTriggerBox;
    //delete _pTestObject3;
    //delete _pTestObject4;
    //delete _pTestObject4;

    // Todo :: 현재 어플리케이션에서 먼저 사운드 시스템을 먼저 릴리즈를 하고 있어서
    // 사운드 릴리즈에 실패하고 있다.

    //delete _pTestClip;
}
