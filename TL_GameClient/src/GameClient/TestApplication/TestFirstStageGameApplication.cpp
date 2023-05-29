#include "GameClient/TestApplication/TestFirstStageGameApplication.h"

#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"

using namespace TL_GameEngine;

void TestFirstStageGameApplication::OnApplicationStart()
{
    GameApplication::OnApplicationStart();

    m_Scene = new Scene(TEXT("MainScene"));
    GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

    m_StageGO = GameObject::Spawn(m_Scene);
    auto _stageStaticMeshRenderer = m_StageGO->AddComponent<StaticMeshRenderer>();
    // TODO
    // _stageStaticMeshRenderer->SetMesh();
    // _stageStaticMeshRenderer->SetMaterial();

    m_Player = GameObject::Spawn(m_Scene);
    m_Player->GetComponent<Transform>()->SetWorldPosition({0, 0, 0});
    auto _playerStaticMeshRenderer = m_Player->AddComponent<StaticMeshRenderer>();
    // TODO
    // _playerStaticMeshRenderer->SetMesh();
    // _playerStaticMeshRenderer->SetMaterial();

    auto _cameraGO = GameObject::Spawn(m_Scene);
    _cameraGO->SetParent(m_Player);
    _cameraGO->GetComponent<Transform>()->SetLocalPosition({0, 5, 0});
    _cameraGO->AddComponent<Camera>();
    _cameraGO->AddComponent<AudioListener>();

    m_SoundSource1 = GameObject::Spawn(m_Scene);
    m_SoundSource1->AddComponent<AudioSource>();
}

void TestFirstStageGameApplication::OnApplicationTick()
{
    GameApplication::OnApplicationTick();
}

void TestFirstStageGameApplication::OnApplicationEnd()
{
    GameApplication::OnApplicationEnd();
}
