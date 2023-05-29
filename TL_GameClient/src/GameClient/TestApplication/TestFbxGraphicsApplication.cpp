#include "GameClient/TestApplication/TestFbxGraphicsApplication.h"

#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/GameFramework/Component/FreeLookCamera.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"
#include "GameEngine/Resource/Resources.h"
#include "GameEngine/Util/UELevelImporter.h"

using namespace TL_GameEngine;

void TestFbxGraphicsApplication::OnApplicationStart()
{
    GameApplication::OnApplicationStart();

    m_Scene = new Scene(TEXT("MainScene"));
    GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

    m_CameraGO = GameObject::Spawn(m_Scene, TEXT("MainCamera"));
    m_CameraGO->SetTag(GameWorld::GetInstance()->FindGameObjectTag(TEXT("MainCamera")));
    m_CameraGO->GetComponent<Transform>()->SetWorldPosition({0, 0, -10});
    m_CameraGO->AddComponent<Camera>();
    m_CameraGO->AddComponent<FreeLookCamera>();

    // Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("chracter/fbx/low_2_ani.fbx"));
    // Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Stage_5_7/Stage_5_7.fbx"));
    // Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Machine_01/Machine_01.fbx"));
	Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Joyful_Jump.TL"));
    // Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("Chapter1_1Stage.fbx"));
    //Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Joyful_Jump.fbx"));

    m_DirectionalLightGO = GameObject::Spawn(m_Scene, TEXT("DirectionalLight"));
    m_DirectionalLightGO->GetComponent<Transform>()->SetWorldPosition({0, 0, 0});
    m_DirectionalLightGO->AddComponent<DirectionalLight>();

    UELevelImporter::Import(m_Scene, Resources::GetConcretedPath(
                                {Resources::GetResourcesRootPath(), TEXT("_DevelopmentAssets/UELevelImporter/chapter2_2.txt")}));
}

void TestFbxGraphicsApplication::OnApplicationTick()
{
    GameApplication::OnApplicationTick();
}

void TestFbxGraphicsApplication::OnApplicationEnd()
{
    GameApplication::OnApplicationEnd();
}
