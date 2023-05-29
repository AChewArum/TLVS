#include "Stage29.h"

#include "GameClient/Components/InputController.h"
#include "GameClient/Components/CameraController.h"
#include "GameClient/Components/ObjectControl.h"
#include "GameClient/Components/CharacterMovement.h"
#include "GameClient/Components/Door.h"
#include "GameClient/Components/ObjectAttribute.h"
#include "GameClient/Components/ShiftController.h"
#include "GameClient/Components/StateMachine.h"
#include "GameClient/Components/TriggerSwitch.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameObjectHelper.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"
#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/Resource/Resources.h"
#include "GameEngine/GameFramework/Component/BoxCollider.h"
#include "GameEngine/GameFramework/Component/MeshCollider.h"
#include "GameEngine/Gameframework/Component/RigidBody.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/Resource/Resources.h"
#include "Physics/Metal/PhysicsMaterial.h"

using namespace TL_GameEngine;

void Stage29::OnApplicationStart()
{
	GameApplication::OnApplicationStart();

	SetPhysicsLayers();
	m_Scene = new Scene(TEXT("StageScene"));
	GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

	const auto _lightObj = GameObject::Spawn(m_Scene);
	_lightObj->AddComponent<DirectionalLight>();
	_lightObj->SetName(TEXT("Light"));


	auto _stageObj = SpawnStageObject();
	m_PlayerObj = SpawnPlayer();

	auto _swt1 = SpawnSwitch({-6, 3, 0.5}, TEXT("_DevelopmentAssets/Model/ButtonFloor.TL"));
	auto _swt2 = SpawnSwitch({-58, 3, 0.5}, TEXT("_DevelopmentAssets/Model/ButtonFloor.TL"));

	auto _cube = GameObject::Spawn(m_Scene);
	auto _cubemdl = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/cube.TL"));
	_cubemdl->SetParent(_cube);
	_cube->m_Transform.SetWorldPosition({-15, 30, 0.5});
	_cube->SetName(TEXT("cube1"));
	_cubemdl->m_Transform.SetLocalPosition({0, 0, 0});
	_cubemdl->m_Transform.SetWorldScale({0.05, 0.05, 0.05});
	_cube->AddComponent<BoxCollider>();
	auto _boxinfo3 = _cube->GetComponent<BoxCollider>();
	_boxinfo3->SetSize(15, 15, 15);

	TL_Physics::PhysicsMaterialInfo _pmi;
	_pmi._bounciness = 0;
	_pmi._bounceCombine = TL_Physics::eBounceCombine::Minimum;
	auto _pm = PhysicsSystem::Instance().CreatePhysicsMaterial(_pmi);

	
	_cube->AddComponent<RigidBody>();
	auto _rigidInfo = _cube->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo._mass = 1.0f;
	_rigidInfo._useGravity = true;
	_rigidInfo._constraints._rotation._y = true;
	_rigidInfo._constraints._rotation._x = true;
	_rigidInfo._constraints._rotation._z = true;
	_rigidInfo._constraints._position._x = true;
	_rigidInfo._constraints._position._z = true;
	_rigidInfo._interpolate = TL_Physics::eInterpolate::Interpolate;
	_rigidInfo._collisionDetection = TL_Physics::eCollisionDetection::Continuous;
	_cube->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);
	_cube->SetLayer(3);

	auto _cube2 = GameObject::Spawn(m_Scene);
	auto _cubemdl2 = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/cube.TL"));
	_cubemdl2->SetParent(_cube2);
	_cube2->m_Transform.SetWorldPosition({-47, 30, 0.5});
	_cube2->SetName(TEXT("cube2"));
	_cubemdl2->m_Transform.SetLocalPosition({0, 0, 0});
	_cubemdl2->m_Transform.SetWorldScale({0.05, 0.05, 0.1});
	_cube2->AddComponent<BoxCollider>();
	auto _boxinfo2 = _cube2->GetComponent<BoxCollider>();
	_boxinfo2->SetSize(10, 10, 20);
	_cube2->AddComponent<RigidBody>();
	auto _rigidInfo2 = _cube2->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo2._mass = 1.0f;
	_rigidInfo2._useGravity = true;
	_rigidInfo2._constraints._rotation._y = true;
	_rigidInfo2._constraints._rotation._x = true;
	_rigidInfo2._constraints._rotation._z = true;
	_rigidInfo2._constraints._position._x = true;
	_rigidInfo2._constraints._position._z = true;
	_rigidInfo2._interpolate = TL_Physics::eInterpolate::Interpolate;
	_rigidInfo2._collisionDetection = TL_Physics::eCollisionDetection::Continuous;
	_cube2->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo2);

	TL_Physics::PhysicsMaterialInfo _pmi2;
	_pmi2._bounciness = 0;
	auto _pm2 = PhysicsSystem::Instance().CreatePhysicsMaterial(_pmi2);
	_boxinfo2->SetMaterial(_pm2);



	_cube2->SetLayer(3);

	auto _cube3 = GameObject::Spawn(m_Scene);
	auto _cubemdl3 = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/cube.TL"));
	_cubemdl3->SetParent(_cube3);
	_cube3->m_Transform.SetWorldPosition({-36, 6, 0.5});
	_cube3->SetName(TEXT("cube"));
	_cubemdl3->m_Transform.SetLocalPosition({0, 0, 0});
	_cubemdl3->m_Transform.SetWorldScale({0.2, 0.01, 0.05});
	_cube3->AddComponent<BoxCollider>();
	auto _boxinfo = _cube3->GetComponent<BoxCollider>();
	_boxinfo->SetSize(40, 10, 10);
	_cube3->AddComponent<RigidBody>();
	auto _rigidInfo3 = _cube3->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo3._mass = 4.0f;
	_rigidInfo3._useGravity = true;
	_rigidInfo3._constraints._rotation._y = true;
	_rigidInfo3._constraints._rotation._x = true;
	_rigidInfo3._constraints._position._x = true;
	_rigidInfo3._constraints._position._y = true;
	_rigidInfo3._constraints._position._z = true;
	_rigidInfo3._interpolate = TL_Physics::eInterpolate::Interpolate;
	_rigidInfo3._collisionDetection = TL_Physics::eCollisionDetection::Continuous;
	_cube3->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo3);
	_cube3->SetLayer(1);
}

void Stage29::OnApplicationTick()
{
	GameApplication::OnApplicationTick();
}

void Stage29::OnApplicationEnd()
{
	GameApplication::OnApplicationEnd();
}

GameObject* Stage29::SpawnPlayer()
{
	const auto _tmpPlayer = GameObject::Spawn(m_Scene);
	_tmpPlayer->SetName(TEXT("PlayerObj"));

	//플레이어 모델
	const auto _playerModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/TL_FBXtest_character_LOW.TL"));
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
	_rigidInfo._collisionDetection = TL_Physics::eCollisionDetection::ContinuousSpeculative;
	_tmpPlayer->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);
	_tmpPlayer->SetLayer(1);


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

	_tmpPlayer->m_Transform.SetWorldPosition({0, 50, -20});
	return _tmpPlayer;
}

GameObject* Stage29::SpawnCamera(const tstring& _name) const
{
	const auto _tmpCam = GameObject::Spawn(m_Scene);
	_tmpCam->SetName(_name);
	_tmpCam->SetLayer(0);
	_tmpCam->AddComponent<Camera>();
	return _tmpCam;
}

GameObject* Stage29::SpawnStageObject() const
{
	const auto _tmpobj = GameObject::Spawn(m_Scene);
	_tmpobj->SetName(TEXT("StageObject"));

	const auto _groundModel = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/woodstage.TL"));
	_groundModel->SetParent(_tmpobj);
	_groundModel->m_Transform.SetWorldScale({0.1, 0.1, 0.1});
	_groundModel->m_Transform.SetLocalPosition({0, 0, 0});
	GameObjectHelper::AddStaticMeshColliderRecursive(_groundModel);

	//_tmpobj->AddComponent<BoxCollider>();
	//TL_Physics::BoxColliderInfo _boxColliderInfo;
	//_boxColliderInfo._size = TL_Math::Vector3(800.0f, 40.0f, 800.0f);
	//_boxColliderInfo._center = TL_Math::Vector3(0, 5, 0);
	//_tmpobj->GetComponent<BoxCollider>()->SetBoxColliderInfo(_boxColliderInfo);
	_tmpobj->GetComponent<Transform>()->SetWorldPosition({0, 0, 0});
	_tmpobj->SetLayerRecursive(1);

	return _tmpobj;
}

GameObject* Stage29::SpawnInteractableObject(Vector3 _loc, const tstring& _path)
{
	static int _one = 1;
	static int _ten = 0;
	static int _hund = 0;

	_hund = _ten / 100;
	_ten = (_one / 10) % 10;
	_one = _one % 10;
	std::wstring _str = std::to_wstring(_hund) + std::to_wstring(_ten) + std::to_wstring(_one);
	const auto _tmpObj = GameObject::Spawn(m_Scene);
	_tmpObj->SetName(TEXT("InteractableObj") + _str);

	auto _model = GameObject::Spawn(m_Scene);
	_model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, _path);
	_model->SetParent(_tmpObj);
	_model->m_Transform.SetLocalPosition({0, 0, 0});
	_model->m_Transform.SetWorldScale({0.1, 0.1, 0.1});
	_model->SetName(TEXT("Model"));


	GameObjectHelper::AddStaticMeshColliderRecursive(_model);

	for (auto _child : _model->GetChilds())
	{
		if (_child->GetComponent<MeshCollider>() != nullptr)
		{
			_child->AddComponent<RigidBody>();
			_child->SetParent(nullptr);
			_child->SetLayer(3);
		}
	}


	//_tmpObj->AddComponent<RigidBody>();
	//auto _rigidInfo = _tmpObj->GetComponent<RigidBody>()->GetRigidInfo();
	//_rigidInfo._mass = 4.0f;
	//_rigidInfo._useGravity = true;
	//_rigidInfo._constraints._rotation._y = true;
	//_rigidInfo._constraints._rotation._x = true;
	//_rigidInfo._constraints._rotation._z = true;
	//_tmpObj->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);


	//_tmpObj->SetLayerRecursive(3);

	_tmpObj->AddComponent<ObjectAttribute>();
	_tmpObj->GetComponent<ObjectAttribute>()->Model(_model);


	_tmpObj->m_Transform.SetWorldPosition(_loc);

	return _tmpObj;
}

void Stage29::SetPhysicsLayers()
{
	PhysicsSystem::Instance().SetLayer(0, 0);
	PhysicsSystem::Instance().SetLayer(1, INT32_MAX);
	PhysicsSystem::Instance().SetLayer(2, 0);
	PhysicsSystem::Instance().SetLayer(3, 0b101111);
	PhysicsSystem::Instance().SetLayer(4, 0b110111);
	PhysicsSystem::Instance().SetLayer(5, 0b111111);
}

GameObject* Stage29::SpawnDoor(Vector3 _loc)
{
	const auto _tmpobj = GameObject::Spawn(m_Scene);
	const auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.fbx"));
	_model->m_Transform.SetWorldScale({0.05, 0.05, 0.05});
	_model->SetParent(_tmpobj);

	_tmpobj->AddComponent<Door>();
	_tmpobj->AddComponent<BoxCollider>();
	_tmpobj->GetComponent<BoxCollider>()->SetSize(10, 10, 2);

	_tmpobj->m_Transform.SetWorldPosition(_loc);

	return _tmpobj;
}

GameObject* Stage29::SpawnSwitch(Vector3 _loc, const tstring& _path)
{
	static int _one = 1;
	static int _ten = 0;
	static int _hund = 0;

	_hund = _ten / 100;
	_ten = (_one / 10) % 10;
	_one = _one % 10;
	std::wstring _str = std::to_wstring(_hund) + std::to_wstring(_ten) + std::to_wstring(_one);

	const auto _tmpobj = GameObject::Spawn(m_Scene);
	_tmpobj->SetName(TEXT("TriggerObject") + _str);
	_tmpobj->AddComponent<TriggerSwitch>();

	auto _model = GameObject::Spawn(m_Scene);
	_model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, _path);
	_model->SetParent(_tmpobj);
	_model->m_Transform.SetLocalPosition({0, -5, 0});
	_model->m_Transform.SetWorldScale({0.1, 0.1, 0.1});
	_model->SetName(TEXT("Model"));

	_tmpobj->AddComponent<BoxCollider>();
	_tmpobj->GetComponent<BoxCollider>()->SetSize(10, 10, 10);
	_tmpobj->GetComponent<BoxCollider>()->SetTrigger(true);
	_tmpobj->m_Transform.SetWorldPosition(_loc);
	_tmpobj->SetLayerRecursive(5);
	return _tmpobj;
}
