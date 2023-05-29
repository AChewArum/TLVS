#include "MainTestApp.h"

#include "GameClient/Components/InputController.h"
#include "GameClient/Components/CameraController.h"
#include "GameClient/Components/ObjectControl.h"
#include "GameClient/Components/CharacterMovement.h"
#include "GameClient/Components/ObjectAttribute.h"
#include "GameClient/Components/ShiftController.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"
#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/Resource/Resources.h"
#include "GameEngine/GameFramework/Component/BoxCollider.h"
#include "GameEngine/Gameframework/Component/RigidBody.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"

using namespace TL_GameEngine;

void MainTestApp::OnApplicationStart()
{
	GameApplication::OnApplicationStart();

	PhysicsSystem::Instance().SetLayer(0, 0);
	PhysicsSystem::Instance().SetLayer(1, INT32_MAX);
	PhysicsSystem::Instance().SetLayer(2, 0);
	PhysicsSystem::Instance().SetLayer(3, 0b1111);
	m_Scene = new Scene(TEXT("TestScene"));
	GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

	m_LightGO = GameObject::Spawn(m_Scene);
	m_LightGO->AddComponent<DirectionalLight>();

	m_PlayerObj = GameObject::Spawn(m_Scene);
	m_PlayerObj->SetName(TEXT("PlayerObj"));
	m_PlayerObj->GetComponent<Transform>()->SetWorldPosition({0, 10, 0});

	auto _s = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/TL_FBXtest_character_LOW.fbx"));
	_s->SetParent(m_PlayerObj);
	_s->m_Transform.SetLocalScale({0.1, 0.1, 0.1});
	_s->GetComponent<Transform>()->SetLocalPosition({0, 0, 0});
	_s->SetName(TEXT("PlayerModel"));
	_s->SetLayer(0);

	m_PlayerObj->AddComponent<InputController>();
	m_PlayerObj->AddComponent<CameraController>();
	m_PlayerObj->AddComponent<ObjectControl>();
	m_PlayerObj->AddComponent<ShiftController>();
	m_PlayerObj->AddComponent<CharacterMovement>();
	m_PlayerObj->GetComponent<CharacterMovement>()->PlayerModel(_s);

	m_PlayerObj->AddComponent<BoxCollider>();
	m_PlayerObj->AddComponent<RigidBody>();
	auto _rigdInfo = m_PlayerObj->GetComponent<RigidBody>()->GetRigidInfo();
	_rigdInfo._useGravity = true;
	_rigdInfo._constraints._rotation._y = true;
	_rigdInfo._constraints._rotation._x = true;
	_rigdInfo._constraints._rotation._z = true;
	m_PlayerObj->GetComponent<RigidBody>()->SetRigidInfo(_rigdInfo);
	m_PlayerObj->SetLayer(1);

	m_PlayerObj->m_Transform.SetWorldPosition({0, 0, 0});

	m_PlayerCameraObj = GameObject::Spawn(m_Scene);

	m_PlayerCameraObj->SetName(TEXT("PlayerCamera"));
	m_PlayerCameraObj->GetComponent<Transform>()->SetLocalPosition({0, 3, 0});
	m_PlayerCameraObj->SetLayer(0);
	m_PlayerCameraObj->AddComponent<Camera>();
	m_PlayerCameraObj->SetParent(m_PlayerObj);


	m_HoverCameraObj = GameObject::Spawn(m_Scene);
	m_HoverCameraObj->SetName(TEXT("HoverCamera"));
	m_HoverCameraObj->SetLayer(0);
	m_HoverCameraObj->GetComponent<Transform>()->SetLocalPosition({0, 50, -4});
	m_HoverCameraObj->AddComponent<Camera>();
	m_HoverCameraObj->SetParent(m_PlayerObj);


	m_TargetObj = GameObject::Spawn(m_Scene);
	m_TargetObj->SetName(TEXT("TargetObj"));

	auto _tamd = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.fbx"));
	_tamd->SetParent(m_TargetObj);
	_tamd->m_Transform.SetLocalPosition({0, 0, 0});
	_tamd->m_Transform.SetWorldScale({0.005, 0.005, 0.005});
	_tamd->SetName(TEXT("Model"));

	m_TargetObj->AddComponent<BoxCollider>();
	TL_Physics::BoxColliderInfo _targetInfo;
	_targetInfo._size = TL_Math::Vector3(4.0f, 4.0f, 4.0f);
	m_TargetObj->GetComponent<BoxCollider>()->SetBoxColliderInfo(_targetInfo);
	m_TargetObj->SetLayer(3);

	m_TargetObj->AddComponent<ObjectAttribute>();

	m_TargetObj->m_Transform.SetWorldPosition({0, 5, 0});

	auto _ground = GameObject::Spawn(m_Scene);
	_ground->SetName(TEXT("Ground"));
	/*auto _gf = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Chapter1_1Stage/Chapter1_1Stage.fbx"));
	_gf->SetParent(_ground);
	_gf->m_Transform.SetLocalPosition({ 0,0,0 });
	_gf->SetLayer(0);*/
	_ground->AddComponent<BoxCollider>();
	TL_Physics::BoxColliderInfo _groundinfo;
	_groundinfo._size = TL_Math::Vector3(200.0f, 20.0f, 200.0f);
	_ground->GetComponent<BoxCollider>()->SetBoxColliderInfo(_groundinfo);
	_ground->GetComponent<Transform>()->SetWorldPosition({0, -10.0f, 0});
	_ground->SetLayer(1);
}

void MainTestApp::OnApplicationTick()
{
	GameApplication::OnApplicationTick();
}

void MainTestApp::OnApplicationEnd()
{
	GameApplication::OnApplicationEnd();
}
