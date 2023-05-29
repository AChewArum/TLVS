#include "ObjectControl.h"

#include "ObjectAttribute.h"
#include "PlayerAttribute.h"
#include "GameEngine/GameFramework/Debug.h"
#include "GameEngine\Gameframework\Physics\PhysicsSystem.h"
#include "GameEngine/Gameframework/Physics/RaycastHit.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Collider.h"
#include "GameEngine/Gameframework/Component/RigidBody.h"
#include "Log/Log.h"

using namespace TL_Math;
using namespace TL_GameEngine;

void ObjectControl::PreTick()
{
	ComponentBase::PreTick();
	if (m_IsMovable)
	{
		MoveObject();
	}
}

void ObjectControl::Tick()
{
	ComponentBase::Tick();
}

void ObjectControl::FixedTick()
{
	ComponentBase::FixedTick();
}

void ObjectControl::OnStart()
{
	ComponentBase::OnStart();
	if (m_GameObject->GetChild(TEXT("CamTransform")))
	{
		m_PlayerCameraTransform = m_GameObject->GetChild(TEXT("CamTransform"))->GetComponent<TL_GameEngine::Transform>();
	}
}

GameObject* ObjectControl::GetGrabTarget()
{
	Vector3 _dir = m_PlayerCameraTransform->GetForwardDirection();
	_dir.Normalize();
	Vector3 _origin = m_PlayerCameraTransform->GetWorldPosition();
	RaycastHit _t;
	auto _dir2 = _dir * 5000.0f;
	Debug::DrawLine(_origin, _dir2, 2.0f);

	if (PhysicsSystem::Instance().Raycast(_origin, _dir, _t, 5000.0f, 0b001100))
	{
		GameObject* _obj = _t._collider->GetGameObject();
		if (!(_obj->GetComponent<ObjectAttribute>())) return nullptr;
		if (!(m_GrabbedObjectTransform = _obj->GetComponent<Transform>())) return nullptr;
		Log::Client_Debug("RayHit");
		return _obj;
	}
	Log::Client_Debug("RayFail");
	return nullptr;
}

void ObjectControl::SetGrab()
{
	if (m_IsGrabOn)
	{
		DetachObject();
	}
	else
	{
		if (auto _obj = GetGrabTarget())
		{
			AttachObject(_obj);
		}
	}
}

void ObjectControl::SetGrabDistance(bool _plus)
{
	if (_plus)
	{
		m_Distance = std::max<float>(m_Distance + m_Delta, m_MaxDistance);
	}
	else
	{
		m_Distance = std::max<float>(m_Distance - m_Delta, m_MinDistance);
	}
}

void ObjectControl::RotateObject(TL_Math::Vector3 _dir)
{
	if (m_GrabbedObjectTransform == nullptr) return;

	/*
	 *	gtm = 잡은 물체의 매트릭스
	 *	ctm = 캐릭터 카메라의 매트릭스
	 *	citm = 캐릭터의 매트릭스의 역
	 */

	auto _gtm = m_GrabbedObjectTransform->GetWorldTM();
	auto _ctm = m_PlayerCameraTransform->GetWorldTM();
	Matrix _citm;
	_ctm.Invert(_citm);

	auto _multTM = _gtm * _citm;

	Vector3 _mScl;
	Quaternion _mRot;
	Vector3 _mPos;

	_multTM.Decompose(_mScl, _mRot, _mPos);

	Quaternion _InputDir = Quaternion::CreateFromYawPitchRoll(_dir * 0.05);
	_InputDir.Inverse(_InputDir);

	auto _resultRot = _mRot * _InputDir;

	Matrix _tmpTM = Matrix::CreateScale(_mScl) * Matrix::CreateFromQuaternion(_resultRot)
		* Matrix::CreateTranslation(_mPos);

	_tmpTM = _tmpTM * _ctm;

	m_GrabbedObjectTransform->SetWorldTM(_tmpTM);

	m_GrabbedObjectTransform->GetWorldTM();

	//auto _rorot = m_GrabbedObjectTransform->GetWorldRotation();
	//_rorot = _rorot * Quaternion::CreateFromAxisAngleDeg(Vector3::Up, 10);
	//m_GrabbedObjectTransform->SetWorldRotation(_rorot);
	Log::Client_Debug("Test");
}

void ObjectControl::AttachObject(GameObject* _target)
{
	auto _objAtt = _target->GetComponent<ObjectAttribute>();
	auto _rigid = _target->GetComponent<RigidBody>()->GetRigidInfo();
	if (_objAtt->Grabable())
	{
		_objAtt->IsGrabbed(true);
		m_IsGrabOn = true;
		m_IsRotateOn = _target->GetComponent<ObjectAttribute>()->Rotatble();
		_objAtt->IsGravity(_rigid._useGravity);
		_objAtt->Mass(_rigid._mass);
		_rigid._useGravity = false;
		_rigid._mass = 0;
		TL_Physics::Constraints _cont;
		_cont._rotation._x = true;
		_cont._rotation._y = true;
		_cont._rotation._z = true;
		_target->GetComponent<RigidBody>()->SetConstraints(_cont);
		_target->GetComponent<RigidBody>()->SetRigidInfo(_rigid);
		_target->GetComponent<RigidBody>()->SetAngularVelocity({ 0, 0, 0 });
		_target->GetComponent<RigidBody>()->SetLinearVelocity({ 0, 0, 0 });
		m_IsMovable = true;
		m_GrabbedObject = _target;
	}
}

void ObjectControl::DetachObject()
{
	auto _objAtt = m_GrabbedObject->GetComponent<ObjectAttribute>();
	auto _rigid = m_GrabbedObject->GetComponent<RigidBody>()->GetRigidInfo();
	if (_objAtt->Grabable())
	{
		_objAtt->IsGrabbed(false);
		m_IsGrabOn = false;
		m_IsRotateOn = m_GrabbedObject->GetComponent<ObjectAttribute>()->Rotatble();
		_rigid._useGravity = _objAtt->IsGravity();
		_rigid._mass = _objAtt->Mass();
		m_GrabbedObject->GetComponent<RigidBody>()->SetRigidInfo(_rigid);
		TL_Physics::Constraints _cont;
		_cont._rotation._x = false;
		_cont._rotation._y = false;
		_cont._rotation._z = false;
		m_GrabbedObject->GetComponent<RigidBody>()->SetConstraints(_cont);

		m_GrabbedObject->GetComponent<RigidBody>()->SetAngularVelocity({0, 0, 0});
		m_GrabbedObject->GetComponent<RigidBody>()->SetLinearVelocity({0, 0, 0});
		m_GrabbedObject->SetLayerRecursive(1);
		m_IsMovable = false;
		m_GrabbedObject = nullptr;
	}
}

void ObjectControl::MoveObject()
{
	auto _move = m_GrabbedObject->GetComponent<RigidBody>();

	Vector3 _dir = m_PlayerCameraTransform->GetForwardDirection();
	_dir.Normalize();
	Vector3 _origin = m_PlayerCameraTransform->GetWorldPosition();

	m_TargetLoc = _origin + (_dir * m_TargetLength);


	auto _targetDir = m_TargetLoc - m_GrabbedObject->m_Transform.GetWorldPosition();

	auto _dist = _targetDir.Length();

	RaycastHit _hit;


	auto _objLoc = m_GrabbedObject->m_Transform.GetWorldPosition();
	auto _dest = _targetDir;
	_dest.Normalize();


	auto _rayDist = _dist;

	if (PhysicsSystem::Instance().Raycast(_objLoc, _dest, _hit, _rayDist, 0b010))
	{
		_targetDir = _hit._point - m_GrabbedObject->m_Transform.GetWorldPosition();
		_targetDir.Normalize(_targetDir);
		_targetDir= _targetDir * (_hit._distance - 5);
		Log::Client_Debug("Blocked!");
	}

	_move->SetLinearVelocity(_targetDir);
}
