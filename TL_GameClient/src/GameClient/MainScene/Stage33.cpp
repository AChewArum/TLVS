#include "Stage33.h"

#include "GameClient/Components/AnimationController.h"
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
#include "GameEngine/GameFramework/GameObjectHelper.h" =
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"
#include "GameEngine/GameFramework/Component/AudioListener.h"
#include "GameEngine/GameFramework/Component/AudioSource.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/Resource/Resources.h"
#include "GameEngine/GameFramework/Component/BoxCollider.h"
#include "GameEngine/GameFramework/Component/CapsuleCollider.h"
#include "GameEngine/GameFramework/Component/MeshCollider.h"
#include "GameEngine/Gameframework/Component/RigidBody.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/GameFramework/Component/Light/SpotLight.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/Resource/Resources.h"
#include "Physics/Metal/PhysicsMaterial.h"

void Stage33::OnApplicationStart()
{
	GameApplication::OnApplicationStart();

	SetPhysicsLayers();
	m_Scene = new Scene(TEXT("StageScene"));
	GameWorld::GetInstance()->ReserveLoadScene(m_Scene);

	const auto _lightObj = GameObject::Spawn(m_Scene);


	_lightObj->AddComponent<DirectionalLight>();
	_lightObj->SetName(TEXT("Light"));


	m_PlayerObj = SpawnPlayer();
	
	auto _stageObj = SpawnStageObject();
	auto _stageColliderObject = SpawnStageColliderObject();
}

void Stage33::OnApplicationTick()
{
	GameApplication::OnApplicationTick();
}

void Stage33::OnApplicationEnd()
{
	GameApplication::OnApplicationEnd();
}

GameObject* Stage33::SpawnPlayer()
{
	auto _obj = GameObject::Spawn(m_Scene);
	_obj->SetName(TEXT("Player"));

	//auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/TL_FBXtest_character_LOW.TL"));
	auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/Player"));
	_model->SetParent(_obj);
	_model->SetName(TEXT("PlayerModel"));
	_model->m_Transform.SetWorldScale({0.1, 0.1, 0.1});

	GameObject* _lightGO = GameObject::Spawn(m_Scene);
	_lightGO->SetParent(_model);
	_lightGO->m_Transform.SetLocalPosition(Vector3{0.0f, 100.0f, 20.0f});
	auto _spotLight = _lightGO->AddComponent<SpotLight>();
	_spotLight->SetRange(50);
	_spotLight->SetIntensity(500);
	_spotLight->SetSpot(100.0f);
	_spotLight->SetColor({ 1.0f,1.0f,0.5f });

	_obj->AddComponent<InputController>();
	_obj->AddComponent<CameraController>();
	_obj->AddComponent<ObjectControl>();
	_obj->AddComponent<ShiftController>();
	_obj->AddComponent<CharacterMovement>();
	_obj->GetComponent<CharacterMovement>()->PlayerModel(_model);
	_obj->AddComponent<StateMachine>();

	_obj->AddComponent<CapsuleCollider>();
	auto _cinfo = _obj->GetComponent<CapsuleCollider>()->GetCapsuleColliderInfo();
	_obj->GetComponent<CapsuleCollider>()->SetCapsuleColliderInfo(_cinfo);

	//애니메이션 부분
	_obj->AddComponent<AnimationController>();

	_obj->AddComponent<RigidBody>();

	auto _rigidInfo = _obj->GetComponent<RigidBody>()->GetRigidInfo();
	_rigidInfo._mass = 4.0f;
	_rigidInfo._useGravity = true;
	_rigidInfo._constraints._rotation._y = true;
	_rigidInfo._constraints._rotation._x = true;
	_rigidInfo._constraints._rotation._z = true;
	_rigidInfo._interpolate = TL_Physics::eInterpolate::Interpolate;
	_rigidInfo._collisionDetection = TL_Physics::eCollisionDetection::ContinuousSpeculative;
	_obj->GetComponent<RigidBody>()->SetRigidInfo(_rigidInfo);
	_obj->SetLayer(2);

	const auto _cam = SpawnCamera(TEXT("PlayerCamera"));
	auto _camTransform = GameObject::Spawn(m_Scene);
	_camTransform->SetName(TEXT("CamTransform"));
	_camTransform->SetParent(_obj);
	_cam->SetParent(_camTransform);

	_cam->m_Transform.SetLocalPosition({0, 0, -12});
	_camTransform->GetComponent<Transform>()->SetLocalPosition({0, 10, 0});

	_obj->m_Transform.SetWorldPosition({0, 20, 0});
	return _obj;
}

GameObject* Stage33::SpawnCamera(const tstring& _name) const
{
	const auto _obj = GameObject::Spawn(m_Scene);
	_obj->SetName(_name);
	_obj->SetLayer(0);
	_obj->AddComponent<Camera>();
	return _obj;
}

GameObject* Stage33::SpawnStageObject()
{
	auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/3-3/3_3_Emissive.fbx"));
	_model->SetName(TEXT("StageObject"));
	_model->m_Transform.SetWorldScale({ 0.1, 0.1, 0.1 });
	_model->m_Transform.SetWorldPosition({ 0, 0, 0 });

	LoadStageObject(_model);
	_model->SetLayerRecursive(1);

    // 전등과 문의 emissive strength를 조절합니다.
    auto _go = _model->FindChildRecursive(TEXT("StaticMeshActor_194"));
    auto _matList = _go->GetComponent<StaticMeshRenderer>()->GetMaterialList();
	_matList[5]->SetEmmisiveStrength(3.0f);
	_matList[6]->SetEmmisiveStrength(1.5f);

    return _model;
}

GameObject* Stage33::SpawnStageColliderObject()
{
	//auto _model = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/TestForMe.TL"));
	auto _modelCollider = Resources::GetInstance()->LoadFBXPrefab(m_Scene, TEXT("_DevelopmentAssets/Model/3-3/StageTest3_3_Collider.fbx"));
	_modelCollider->SetName(TEXT("StageColliderObject"));
	_modelCollider->m_Transform.SetWorldScale({0.1, 0.1, 0.1});
	_modelCollider->m_Transform.SetWorldPosition({0, 0, 0});

	GameObjectHelper::AddStaticMeshColliderRecursive(_modelCollider);
	GameObjectHelper::DisableStaticMeshRendererRecursive(_modelCollider);

	_modelCollider->SetLayerRecursive(1);

	return _modelCollider;
}
 
void Stage33::SetPhysicsLayers()
{
	//무반응
	PhysicsSystem::Instance().SetLayer(0, 0);
	//전부 반응
	PhysicsSystem::Instance().SetLayer(1, INT32_MAX);
	//3d 옵젝
	PhysicsSystem::Instance().SetLayer(2, 0b10110);
	//2d 옵젝
	PhysicsSystem::Instance().SetLayer(3, 0b100000);
	//3d 트리거
	PhysicsSystem::Instance().SetLayer(4, 0b100);
	//2d 트리거
	PhysicsSystem::Instance().SetLayer(5, 0b1000);
	//유리벽
	PhysicsSystem::Instance().SetLayer(6, 0b110);
}

void Stage33::LoadStageObject(GameObject* _stage)
{
	std::vector<TriggerSwitch*> _switch;
	std::vector<Door*> _door;
	std::vector<GameObject*> _objs;
	for (auto _child : _stage->GetChilds())
	{
		auto _name = _child->GetName();
		auto _pos = _child->m_Transform.GetWorldPosition();
		if (_name.find(L"Int") != std::wstring::npos)
		{
			_child->SetParent(nullptr);
			_child->m_Transform.SetLocalPosition({0, 0, 0});
			_child->m_Transform.SetWorldPosition(_pos);
			_child->AddComponent<BoxCollider>();
			_child->GetComponent<BoxCollider>()->SetSize(2, 2, 2);
			_child->AddComponent<ObjectAttribute>();
			_child->GetComponent<ObjectAttribute>()->Model(_child);
			_child->AddComponent<RigidBody>();
			auto _info = _child->GetComponent<RigidBody>()->GetRigidInfo();
			_info._collisionDetection = TL_Physics::eCollisionDetection::Continuous;
			_info._interpolate = TL_Physics::eInterpolate::Interpolate;
			_info._constraints._rotation._x = false;
			_info._constraints._rotation._y = false;
			_info._constraints._rotation._z = false;
			_child->GetComponent<RigidBody>()->SetRigidInfo(_info);
			_child->SetLayer(2);

			const auto _material = new Material();
			TL_Graphics::MaterialDesc matDesc;
			matDesc.baseColor_opcityFilePath = TEXT("_DevelopmentAssets/Model/Wooden_Crate/Wooden Crate_Crate_BaseColor.png");
			_material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);
			Resource<Material> _outRes;
			_outRes.SetPtr(_material);
			std::vector<Resource<Material>> _mat;
			_mat.push_back(_outRes);
			if(_child->GetComponent<StaticMeshRenderer>())
			_child->GetComponent<StaticMeshRenderer>()->SetMaterial(_mat);

			_objs.push_back(_child);
		}
		else if (_name.find(L"Button") != std::wstring::npos)
		{
			_child->SetParent(nullptr);
			_child->m_Transform.SetLocalPosition({0, 0, 0});
			_child->m_Transform.SetWorldPosition(_pos);

			_child->AddComponent<BoxCollider>();
			_child->GetComponent<BoxCollider>()->SetTrigger(true);
			_child->GetComponent<BoxCollider>()->SetSize(60, 60, 60);
			_child->AddComponent<TriggerSwitch>();
			_child->SetLayer(4);
			_switch.push_back(_child->GetComponent<TriggerSwitch>());
			if (_name.find(L"Is") != std::wstring::npos)
			{
				_child->GetComponent<TriggerSwitch>()->Is2D(true);
				_child->SetLayer(5);
			}
		}
		else if (_name.find(L"door") != std::wstring::npos)
		{
			_child->SetParent(nullptr);
			_child->m_Transform.SetLocalPosition({0, 0, 0});
			_child->m_Transform.SetWorldPosition(_pos);

			_child->AddComponent<Door>();
			_child->AddComponent<BoxCollider>();
			_child->GetComponent<BoxCollider>()->SetCenter(0, 0, 0);
			_child->GetComponent<BoxCollider>()->SetSize(3, 3, 3);
			_child->SetLayer(1);

			_door.push_back(_child->GetComponent<Door>());
		}
		else if (_name.find(L"seesaw") != std::wstring::npos)
		{
			_child->SetParent(nullptr);
			_child->m_Transform.SetLocalPosition({0, 0, 0});
			_child->m_Transform.SetWorldPosition(_pos);
			_child->AddComponent<BoxCollider>();
			_child->GetComponent<BoxCollider>()->SetCenter(0, 0, 0);
			_child->GetComponent<BoxCollider>()->SetSize(2, 2, 2);
			_child->AddComponent<RigidBody>();
			auto _info = _child->GetComponent<RigidBody>()->GetRigidInfo();
			_info._useGravity = false;
			_info._collisionDetection = TL_Physics::eCollisionDetection::Continuous;
			_info._mass = 30.0f;
			_info._constraints._rotation._y = true;
			_info._constraints._rotation._z = true;
			_info._constraints._position._x = true;
			_info._constraints._position._y = true;
			_info._constraints._position._z = true;
			_info._interpolate = TL_Physics::eInterpolate::Interpolate;
			_child->GetComponent<RigidBody>()->SetRigidInfo(_info);
			_child->SetLayer(1);
		}
		else if (_name.find(L"glass") != std::wstring::npos)
		{
			_child->SetParent(nullptr);
			_child->AddComponent<BoxCollider>();
			_child->SetLayer(6);
		}
	}
	for (auto _child : _switch)
	{
		auto _name = _child->GetGameObject()->GetName();
		auto _index = _name.find(L"D");
		auto _str = _name.substr(_index + 1, 2);

		auto _doorObj = [_door, _str]() -> GameObject*
		{
			for (auto _child : _door)
			{
				auto _doorname = _child->GetGameObject()->GetName();
				if (_doorname.find(_str) != std::string::npos)
				{
					return _child->GetGameObject();
				}
			}
			return nullptr;
		};
		size_t _index2;
		std::wstring _str2;

		auto _targetObj = [_objs](std::wstring _targetname) -> GameObject*
		{
			for (auto _child : _objs)
			{
				auto _objName = _child->GetName();
				if (_objName.find(_targetname) != std::string::npos)
				{
					return _child;
				}
			}
			return nullptr;
		};

		_child->SetDoor(_doorObj());
		if (_name.find(L"Obj") != std::wstring::npos)
		{
			_index2 = _name.find(L"Obj");
			_str2 = _name.substr(_index2, 6);
			_child->SetTarget(_targetObj(_str2));
		}
		
	}
}
