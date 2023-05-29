#include "StageTestApp.h"

#include "GameClient/Components/InputController.h"
#include "GameClient/Components/CameraController.h"
#include "GameClient/Components/ObjectControl.h"
#include "GameClient/Components/CharacterMovement.h"
#include "GameClient/Components/Door.h"
#include "GameClient/Components/ObjectAttribute.h"
#include "GameClient/Components/ShiftController.h"
#include "GameClient/Components/StateMachine.h"
#include "GameClient/Components/TriggerSwitch.h"
#include "GameClient/Components/AnimationController.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameObjectHelper.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/Gameframework/Component/Animator.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"
#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/Resource/Resources.h"
#include "GameEngine/GameFramework/Component/BoxCollider.h"
#include "GameEngine/Gameframework/Component/RigidBody.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/Resource/Resources.h"

using namespace TL_GameEngine;

void StageTestApp::OnApplicationStart()
{
	GameApplication::OnApplicationStart();

	SetPhysicsLayers();
	m_Scene = new Scene(TEXT("StageScene"));
	GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

	const auto _lightObj = GameObject::Spawn(m_Scene);
	_lightObj->AddComponent<DirectionalLight>();
	auto _stageObj = SpawnStageObject();
	auto _stageobj = GameObject::Spawn(m_Scene);
	_stageobj->AddComponent<BoxCollider>();
	_stageobj->GetComponent<BoxCollider>()->SetCenter(0, -20, 0);
	_stageobj->GetComponent<BoxCollider>()->SetSize(400, 10, 400);
	m_PlayerObj = SpawnPlayer();
}

void StageTestApp::OnApplicationTick()
{
	GameApplication::OnApplicationTick();
}

void StageTestApp::OnApplicationEnd()
{
	GameApplication::OnApplicationEnd();
}

GameObject* StageTestApp::SpawnPlayer()
{
	const auto _tmpPlayer = GameObject::Spawn(m_Scene);
	_tmpPlayer->SetName(TEXT("PlayerObj"));

	//플레이어 모델
	//const auto _playerModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Joy"));
	const auto _playerModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Player"));
	//const auto _playerModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/ButtonFloor.fbx"));
	//const auto _playerModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Player/Player_idle.fbx"));
	_playerModel->SetParent(_tmpPlayer);
	_playerModel->m_Transform.SetLocalPosition({0, 0, 0});
	_playerModel->m_Transform.SetLocalScale({0.1, 0.1, 0.1});
	_playerModel->SetName(TEXT("PlayerModel"));
	_playerModel->SetLayer(0);

	//조작계 + 이동계
	_tmpPlayer->AddComponent<InputController>();
	_tmpPlayer->AddComponent<CameraController>();
	_tmpPlayer->AddComponent<ObjectControl>();
	_tmpPlayer->AddComponent<ShiftController>();
	_tmpPlayer->AddComponent<CharacterMovement>();
	_tmpPlayer->AddComponent<StateMachine>();
	_tmpPlayer->GetComponent<CharacterMovement>()->PlayerModel(_playerModel);

	//물리 부분
	_tmpPlayer->AddComponent<BoxCollider>();
	_tmpPlayer->AddComponent<RigidBody>();
	auto _rigidInfo = _tmpPlayer->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo._mass = 4.0f;
	_rigidInfo._useGravity = true;
	_rigidInfo._constraints._rotation._y = true;
	_rigidInfo._constraints._rotation._x = true;
	_rigidInfo._constraints._rotation._z = true;
	_rigidInfo._interpolate = TL_Physics::eInterpolate::Interpolate;
	_tmpPlayer->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);
	_tmpPlayer->SetLayer(1);

	//애니메이션 부분
	_tmpPlayer->AddComponent<AnimationController>();

	const auto _playerCam = SpawnCamera(TEXT("PlayerCamera"));
	const auto _thirdCam = SpawnCamera(TEXT("HoverCamera"));
	auto _camTransform = GameObject::Spawn(m_Scene);
	_camTransform->SetName(TEXT("CamTransform"));
	_camTransform->SetParent(_tmpPlayer);
	_playerCam->SetParent(_camTransform);
	_thirdCam->SetParent(_camTransform);
	_camTransform->m_Transform.SetLocalPosition({0, 8, 0});
	_playerCam->GetComponent<Transform>()->SetLocalPosition({0, 0, 0});
	_thirdCam->GetComponent<Transform>()->SetLocalPosition({0, 5, -20});

	_tmpPlayer->m_Transform.SetWorldPosition({0, 20, 0});
	return _tmpPlayer;
}

GameObject* StageTestApp::SpawnCamera(const tstring& _name) const
{
	const auto _tmpCam = GameObject::Spawn(m_Scene);
	_tmpCam->SetName(_name);
	_tmpCam->SetLayer(0);
	_tmpCam->AddComponent<Camera>();
	return _tmpCam;
}

GameObject* StageTestApp::SpawnStageObject() const
{
	const auto _ground = GameObject::Spawn(m_Scene);
	_ground->SetName(TEXT("StageObject"));

	const auto _groundModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Chapter3_1Stage/Chapter3_1Stage.fbx"));
	_groundModel->SetParent(_ground);
	_groundModel->m_Transform.SetWorldScale({0.1, 0.1, 0.1});
	_groundModel->m_Transform.SetLocalPosition({0, 0, 0});
	//GameObjectHelper::AddStaticMeshColliderRecursive(_groundModel);

	_ground->AddComponent<BoxCollider>();
	TL_Physics::BoxColliderInfo _boxColliderInfo;
	_boxColliderInfo._size = TL_Math::Vector3(800.0f, 40.0f, 800.0f);
	_boxColliderInfo._center = TL_Math::Vector3(0, 5, 0);
	_ground->GetComponent<BoxCollider>()->SetBoxColliderInfo(_boxColliderInfo);
	_ground->GetComponent<Transform>()->SetWorldPosition({0, -10, 0});
	_ground->SetLayerRecursive(1);

	return _ground;
}

GameObject* StageTestApp::SpawnInteractableObject(Vector3 _loc)
{
	const auto _tmpObj = GameObject::Spawn(m_Scene);
	_tmpObj->SetName(TEXT("TargetObj"));

	const auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Player/Player_idle.TL"));
	_model->SetParent(_tmpObj);
	_model->m_Transform.SetLocalPosition({0, 0, 0});
	_model->m_Transform.SetWorldScale({0.01, 0.01, 0.01});
	_model->SetName(TEXT("Model"));


	_tmpObj->AddComponent<BoxCollider>();
	_tmpObj->AddComponent<RigidBody>();
	auto _rigidInfo = _tmpObj->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo._mass = 4.0f;
	_rigidInfo._useGravity = true;
	_rigidInfo._constraints._rotation._y = true;
	_rigidInfo._constraints._rotation._x = true;
	_rigidInfo._constraints._rotation._z = true;
	_tmpObj->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);
	TL_Physics::BoxColliderInfo _boxColliderInfo;
	_boxColliderInfo._size = TL_Math::Vector3(4.0f, 4.0f, 4.0f);
	_tmpObj->GetComponent<BoxCollider>()->SetBoxColliderInfo(_boxColliderInfo);

	_tmpObj->SetLayer(3);

	//_tmpObj->AddComponent<BoxCollider>();

	//_boxColliderInfo._size = TL_Math::Vector3(4.0f, 4.0f, 4.0f);
	//_tmpObj->GetComponent<BoxCollider>()->SetBoxColliderInfo(_boxColliderInfo);
	//_tmpObj->SetLayer(3);

	_tmpObj->AddComponent<ObjectAttribute>();
	_tmpObj->GetComponent<ObjectAttribute>()->Model(_model);


	_tmpObj->m_Transform.SetWorldPosition(_loc);

	return _tmpObj;
}

void StageTestApp::SetPhysicsLayers()
{
	PhysicsSystem::Instance().SetLayer(0, 0);
	PhysicsSystem::Instance().SetLayer(1, INT32_MAX);
	PhysicsSystem::Instance().SetLayer(2, 0);
	PhysicsSystem::Instance().SetLayer(3, 0b101111);
	PhysicsSystem::Instance().SetLayer(4, 0b110111);
	PhysicsSystem::Instance().SetLayer(5, 0b111111);
}

GameObject* StageTestApp::SpawnDoor(Vector3 _loc)
{
	const auto _tmpobj = GameObject::Spawn(m_Scene);
	const auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.TL"));
	_model->m_Transform.SetWorldScale({0.05, 0.05, 0.05});
	_model->SetParent(_tmpobj);
	_tmpobj->AddComponent<Door>();
	_tmpobj->m_Transform.SetWorldPosition(_loc);
	return _tmpobj;
}

GameObject* StageTestApp::SpawnSwitch(Vector3 _loc)
{
	const auto _object = GameObject::Spawn(m_Scene);
	_object->SetName(TEXT("TriggerObject"));

	_object->AddComponent<TriggerSwitch>();
	_object->AddComponent<BoxCollider>();
	_object->GetComponent<BoxCollider>()->SetSize(10, 2, 10);
	_object->GetComponent<BoxCollider>()->SetTrigger(true);
	_object->m_Transform.SetWorldPosition(_loc);
	_object->SetLayerRecursive(5);
	return _object;
}
