#include "GameClient/TestApplication/TestGraphicsApplication.h"

#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/GameFramework/Component/FreeLookCamera.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/Resource/Resources.h"

#include "Application/GameTime.h"
#include "Graphics/RenderSystem.h"
#include "Log/Log.h"
#include "Math/TL_Math.h"

using namespace TL_GameEngine;
using namespace TL_Application;
using namespace TL_Graphics;
using namespace TL_Math;

void TestGraphicsApplication::OnApplicationStart()
{
    struct Vertex
    {
        Vector3 position;
    };

    std::vector<Vertex> vertices(8);
    //앞면
    vertices[0].position = Vector3{-0.5f, +0.5f, -0.5f};
    vertices[1].position = Vector3{+0.5f, +0.5f, -0.5f};
    vertices[2].position = Vector3{-0.5f, -0.5f, -0.5f};
    vertices[3].position = Vector3{+0.5f, -0.5f, -0.5f};
    //뒷면
    vertices[4].position = Vector3{-0.5f, +0.5f, +0.5f};
    vertices[5].position = Vector3{+0.5f, +0.5f, +0.5f};
    vertices[6].position = Vector3{-0.5f, -0.5f, +0.5f};
    vertices[7].position = Vector3{+0.5f, -0.5f, +0.5f};

    TL_Graphics::VertexAttribute vertexAttribute;
    vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
    vertexAttribute.AddData(vertices.data(), sizeof(Vertex) * vertices.size());

    std::vector<uint> indicies =
    {
        //front
        0, 1, 2,
        1, 3, 2,
        //back
        5, 4, 7,
        4, 6, 7,

        //up
        4, 5, 0,
        5, 1, 0,
        //down
        2, 3, 6,
        3, 7, 6,

        //left
        4, 0, 6,
        0, 2, 6,
        //right
        1, 5, 3,
        5, 7, 3
    };

    m_Mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies.data(), indicies.size(), L"Shader/BoxCameraVS.hlsl");
    m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

    GetRenderSystem()->UpdateWindowSize(1280, 720);

    /* 위까지 형 예제 */

    const auto _mesh = new StaticMesh();
    _mesh->m_Mesh = m_Mesh;

    Resource<StaticMesh> _meshRes;
    _meshRes.SetPtr(_mesh);

    const auto _material = new Material();
    _material->m_Material = m_Material;

    Resource<Material> _materialRes;
    _materialRes.SetPtr(_material);

    // go1

    m_Scene = new Scene(TEXT("MainScene"));
    m_GameObject = GameObject::Spawn(m_Scene);
    m_GameObject->SetName(TEXT("GameObject 1"));
    const auto _goRenderer = m_GameObject->AddComponent<StaticMeshRenderer>();
    _goRenderer->SetMesh(_meshRes);
    _goRenderer->SetMaterial({_materialRes});

    // go2

    m_GameObject2 = GameObject::Spawn(m_Scene);
    m_GameObject2->SetName(TEXT("GameObject 2"));
    m_GameObject2->GetComponent<Transform>()->SetWorldPosition({5, 0, 5});
    const auto _go2Renderer = m_GameObject2->AddComponent<StaticMeshRenderer>();
    _go2Renderer->SetMesh(_meshRes);
    _go2Renderer->SetMaterial({_materialRes});

    // go3, go3_child

    m_GameObject3 = GameObject::Spawn(m_Scene);
    m_GameObject3->SetName(TEXT("GameObject 3"));
    const auto _go3Renderer = m_GameObject3->AddComponent<StaticMeshRenderer>();
    _go3Renderer->SetMesh(_meshRes);
    _go3Renderer->SetMaterial({_materialRes});

    m_GameObject3_Child = GameObject::Spawn(m_Scene);
    m_GameObject3_Child->SetName(TEXT("GameObject 3_Child"));
    const auto _go3ChildRenderer = m_GameObject3_Child->AddComponent<StaticMeshRenderer>();
    _go3ChildRenderer->SetMesh(_meshRes);
    _go3ChildRenderer->SetMaterial({_materialRes});

    m_GameObject3->GetComponent<Transform>()->SetWorldPosition({-5, 0, -5});
    m_GameObject3_Child->SetParent(m_GameObject3);
    m_GameObject3_Child->GetComponent<Transform>()->SetLocalPosition({0, 0, 0});

    m_CameraGO = GameObject::Spawn(m_Scene);
    m_CameraGO->GetComponent<Transform>()->SetWorldPosition({0, 0, -10});
    m_CameraGO->AddComponent<Camera>();
    m_CameraComponent = m_CameraGO->AddComponent<FreeLookCamera>();

    GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

    Log::Client_Debug("hello! test log!");
}

void TestGraphicsApplication::OnApplicationTick()
{
    GameApplication::OnApplicationTick();

    const auto _go2Transform = m_GameObject2->GetComponent<Transform>();
    _go2Transform->SetWorldScale(
        Vector3::One * 2.5 * sinf(GameTime::GetGameTime() * 1.5f)
    );

    const auto _go3Transform = m_GameObject3->GetComponent<Transform>();
    _go3Transform->SetWorldRotation(
        _go3Transform->GetWorldRotation() * Quaternion::CreateFromAxisAngle(Vector3::Forward, 1 * GameTime::GetDeltaTime())
    );

    const auto _go3ChildTransform = m_GameObject3_Child->GetComponent<Transform>();
    _go3ChildTransform->SetLocalPosition(
        5 * Vector3::Up * sinf(GameTime::GetGameTime() * 1.5f)
    );
}
