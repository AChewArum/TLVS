#include "GameClient\TestPhysicsObject.h"

#include "GameEngine\GameFramework\GameObject.h"
#include "GameEngine\GameFramework\Scene.h"

#include "GameEngine\GameFramework\Component\Transform.h"
#include "GameEngine\GameFramework\Component\BoxCollider.h"
#include "GameEngine\GameFramework\Component\SphereCollider.h"
#include "GameEngine\GameFramework\Component\CapsuleCollider.h"
#include "GameEngine\GameFramework\Component\MeshCollider.h"
#include "GameEngine\GameFramework\Component\RigidBody.h"
#include "GameEngine\GameFramework\Component\AudioReverbZones.h"
#include "GameEngine\GameFramework\Component\StaticMeshRenderer.h"

#include "GameEngine\GameFramework\Physics\PhysicsSystem.h"
#include "GameEngine\GameFramework\Physics\RaycastHit.h"
#include "GameEngine\GameFramework\LayerMask.h"

#include "Application\Input.h"
#include "Application/GameTime.h"

#include "Log/Log.h"


using namespace TL_GameEngine;
using namespace TL_Application;

TestPhysicsObject::TestPhysicsObject(Scene* scene, float x, float y, float z, bool isRigid, eColType colType)
{
	m_pGameObject = GameObject::Spawn(scene);

	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_pTransform->SetWorldPosition({x, y, z});

	switch (colType) 
	{
		case eColType::BOX:
		{
			m_pBoxCollider = m_pGameObject->AddComponent<BoxCollider>();
			break;
		}
		case eColType::SPHERE:
		{
			m_pSphereCollider = m_pGameObject->AddComponent<SphereCollider>();
			break;
		}
		case eColType::CAPSULE:
		{
			m_pCapsuleCollider = m_pGameObject->AddComponent<CapsuleCollider>();
			break;
		}
		case eColType::MESH:
		{
			m_pMeshCollider = m_pGameObject->AddComponent<MeshCollider>();
			break;
		}
	}

	if (isRigid)
	{
		m_pRigidBody = m_pGameObject->AddComponent<RigidBody>();

	}
}

void TestPlayerComponent::OnStart()
{
	m_GameObject->SetName(TEXT("TestPlayer"));
	m_pRigidBody = m_GameObject->GetComponent<RigidBody>();

}

void TestPlayerComponent::Tick()
{
	ComponentBase::Tick();

	if (Input::GetKeyPress(EVirtualKey::Key_W))
	{
		m_pRigidBody->AddForce(m_GameObject->GetComponent<Transform>()->GetForwardDirection());
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_S))
	{
		m_pRigidBody->AddForce(m_GameObject->GetComponent<Transform>()->GetForwardDirection() * -1);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_A))
	{
		m_pRigidBody->AddForce(m_GameObject->GetComponent<Transform>()->GetRightDirection() * -1);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_D))
	{
		m_pRigidBody->AddForce(m_GameObject->GetComponent<Transform>()->GetRightDirection());
	}
	if (Input::GetKeyDown(EVirtualKey::Key_Q))
	{
		auto _scale = m_GameObject->GetComponent<Transform>()->GetLocalScale();

		_scale.x++;

		m_GameObject->GetComponent<Transform>()->SetLocalScale(_scale);
	}
	else if (Input::GetKeyDown(EVirtualKey::Key_E))
	{
		auto _scale = m_GameObject->GetComponent<Transform>()->GetLocalScale();

		_scale.x--;

		m_GameObject->GetComponent<Transform>()->SetLocalScale(_scale);
	}
	if (Input::GetKeyDown(EVirtualKey::MouseLeft))
	{
		TL_Math::Vector3 _origin{2, 0.5, 0};
		TL_Math::Vector3 _dir{1, 0, 0};

		LayerMask _layerMask1 = LayerMask::NameToLayer(TEXT("Layer3")) | LayerMask::NameToLayer(TEXT("Layer2"));
		
		bool _ret1 = Physics.Raycast(_origin, _dir, 100, _layerMask1);

		RaycastHit _raycastHit;

		LayerMask _layerMask2 = LayerMask::NameToLayer(TEXT("Layer1"));

		bool _ret2 = Physics.Raycast(_origin, _dir, _raycastHit, 100, _layerMask2);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_X))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetDecayTime(++_properties._decayTime);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_C))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetEarlyDelay(++_properties._earlyDelay);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_V))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetLateDelay(++_properties._lateDelay);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_B))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetHFReference(++_properties._hfReference);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_N))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetHFDecayRatio(++_properties._hfDecayRatio);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_M))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetDiffusion(++_properties._diffusion);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_F))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetDensity(++_properties._density);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_G))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetLowShelfFrequency(++_properties._lowShelfFrequency);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_H))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetLowShelfGain(++_properties._lowShelfGain);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_J))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetHighCut(++_properties._highCut);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_K))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetEarlyLateMix(++_properties._earlyLateMix);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_L))
	{
		auto _properties = m_pReverb->GetReverbProperties();

		m_pReverb->SetWetLevel(++_properties._wetLevel);
	}
	else if (Input::GetKeyDown(EVirtualKey::Key_1))
	{
		for (size_t i = 0; i < m_test1.size(); i++)
		{
			m_test1[i]->SetEnable(true);
			m_test1[i]->GetComponent<MeshCollider>()->SetEnable(false);
			m_test1[i]->GetComponent<MeshCollider>()->SetEnable(true);
		}

		/*for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					TestPhysicsObject* test = new TestPhysicsObject(_scene, i - 3, j + 2, k - 3, true, eColType::MESH);
					test->m_pMeshCollider->SetConvex(true);
					test->m_pMeshCollider->SetMesh(hahaha);
					auto* _goRenderer = test->m_pGameObject->AddComponent<StaticMeshRenderer>();
					_goRenderer->SetMesh(hahaha);
					_goRenderer->SetMaterial(hahaha2);
					test->m_pGameObject->SetEnable(false);
				}
			}
		}*/
	}
	else if (Input::GetKeyDown(EVirtualKey::Key_2))
	{
		for (size_t i = 0; i < m_test2.size(); i++)
		{
			m_test2[i]->SetEnable(true);
		}
	}
}

void TestPlayerComponent::OnCollisionEnter(TL_GameEngine::Collision& collision)
{
	__super::OnCollisionEnter(collision);
	//DebugHelper::PrintDebugString(TEXT("Enter \n"));
}

void TestPlayerComponent::OnCollisionStay(TL_GameEngine::Collision& collision)
{
	//DebugHelper::PrintDebugString(TEXT("Stay \n"));
}

void TestPlayerComponent::OnCollisionExit(TL_GameEngine::Collision& collision)
{
	//DebugHelper::PrintDebugString(TEXT("Exit \n"));
}

void TestTriggerBoxComponent::OnStart()
{
	m_GameObject->SetName(TEXT("TestTrigger"));
	m_BoxCollider = m_GameObject->GetComponent<BoxCollider>();
	m_MeshRenderer = m_GameObject->GetComponent<StaticMeshRenderer>();
	m_BoxCollider->SetTrigger(true);
}

void TestTriggerBoxComponent::Tick()
{

}

void TestTriggerBoxComponent::OnTriggerEnter(const TL_GameEngine::Collider& other)
{	
	//m_BoxCollider->SetSize(2, 2, 2);
	m_GameObject->GetComponent<Transform>()->SetLocalScale(Vector3(2, 2, 2));

	//m_MeshRenderer->SetMesh(m_Mesh2);
}

void TestTriggerBoxComponent::OnTriggerStay(const TL_GameEngine::Collider& other)
{
	tstring _test = other.GetGameObject()->GetName();

	_test += TEXT("\n");

	//DebugHelper::PrintDebugString(_test.c_str());

	Log::Client_Debug(StringHelper::ToString(_test.c_str()));
}

void TestTriggerBoxComponent::OnTriggerExit(const TL_GameEngine::Collider& other)
{

	m_GameObject->GetComponent<Transform>()->SetLocalScale(Vector3(1, 1, 1));
	//m_BoxCollider->SetSize(1, 1, 1);
	//m_MeshRenderer->SetMesh(m_Mesh1);
}

void TestCharactorCtrlComponent::OnStart()
{
	m_pControl = m_GameObject->GetComponent<Transform>();
	m_pCamera = m_GameObject->GetChild(0)->GetComponent<Transform>();
}

using namespace TL_Math;

void TestCharactorCtrlComponent::Tick()
{
	ComponentBase::Tick();

	// 마우스로 회전합니다.

	float _rotationPitch = 0;
	float _rotationY = 0;

	if (Input::GetKeyPress(EVirtualKey::MouseRight))
	{
		const auto _mouseDelta = Input::GetMouseMovementDelta();
		_rotationPitch = _mouseDelta.y * 0.1f * 0.01745329252f;
		_rotationY = _mouseDelta.x * 0.1f * 0.01745329252f;


		if (_rotationPitch != 0)
		{
			Vector3 _rightDir = m_pCamera->GetLocalTM().Right();
			Quaternion _camRotation = m_pCamera->GetLocalRotation();

			_camRotation = _camRotation * Quaternion::CreateFromAxisAngle(_rightDir, _rotationPitch);

			m_pCamera->SetLocalRotation(_camRotation);
		}
		if (_rotationY != 0)
		{
			Quaternion _ctrlRotation = m_pControl->GetWorldRotation();

			_ctrlRotation = _ctrlRotation * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotationY);

			m_pControl->SetWorldRotation(_ctrlRotation);
		}
	}
}

void TestAutoMoveComponent::Tick()
{
	auto* _trans = m_GameObject->GetComponent<Transform>();

	Vector3 _target = m_Dir == 1 ? m_Point1 : m_Point2;

	Vector3 _currPos = _trans->GetWorldPosition();

	Vector3 _unit = m_Point1 - m_Point2;
	_unit.Normalize();
	float _dis = TL_Math::Vector3::Distance(_target, _currPos);

	if (_dis < 0.1)
	{
		m_Dir *= -1;
	}

	_trans->SetWorldPosition(_currPos + m_fSpeed * (m_Dir * _unit) * GameTime::GetDeltaTime());
}
