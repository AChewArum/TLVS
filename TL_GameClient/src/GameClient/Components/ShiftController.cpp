#include "ShiftController.h"

#include "Application/Input.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Collider.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/Gameframework/Physics/RaycastHit.h"
#include "ObjectAttribute.h"
#include "ObjectControl.h"
#include "GameEngine/GameFramework/Debug.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/Resource/Resources.h"
#include "Log/Log.h"

using namespace TL_Math;
using namespace TL_GameEngine;

void ShiftController::OnStart()
{
	ComponentBase::OnStart();
	m_MyT = m_GameObject->GetChild(TEXT("CamTransform"))->GetChild(TEXT("PlayerCamera"))->GetComponent<TL_GameEngine::Transform>();
}

void ShiftController::Tick()
{
	ComponentBase::Tick();
}

void ShiftController::Convert2D()
{
	if (SetObject())
	{
		if (m_TargetAtt->IsGrabbed())
		{
			m_GameObject->GetComponent<ObjectControl>()->DetachObject();
		}

		if (!m_TargetAtt->Is2D())
		{
			ShiftScale(m_ImpactLoc);
			m_TargetAtt->Is2D(true);

			m_TargetAtt->GetGameObject()->SetLayer(3);
		}
	}
}

void ShiftController::Convert3D()
{
	if (!SetObject()) return;

	if (m_TargetAtt->Is2D())
	{
		m_TargetAtt->Model()->m_Transform.SetWorldScale(m_OriginScl);
		m_TargetAtt->GetGameObject()->SetLayer(2);
		m_TargetAtt->Is2D(false);
	}
}

void ShiftController::SetVisibilty(bool _on) { }

void ShiftController::SetCollision(bool _on) { }

bool ShiftController::SetObject()
{
	Vector3 _dir = m_MyT->GetForwardDirection();
	_dir.Normalize();
	Vector3 _origin = m_MyT->GetWorldPosition();
	RaycastHit _hitinfo;

	auto _dir2 = _dir * 5000.0f;
	Debug::DrawLine(_origin, _dir2, 2.0f);
	if (PhysicsSystem::Instance().Raycast(_origin, _dir, _hitinfo, 5000.0f, 0b001100))
	{
		GameObject* _obj = _hitinfo._collider->GetGameObject();
		if (!(m_TargetAtt = _obj->GetComponent<ObjectAttribute>()))
			return false;
		if (!(m_ObjT = _obj->GetComponent<Transform>()))
			return false;

		Log::Client_Debug("RayHit");
		m_ImpactLoc = _hitinfo._point;
		return true;
	}

	Log::Client_Debug("RayFail");
	return false;
}

void ShiftController::ShiftScale(Vector3 _impactPoint)
{
	auto& _targetTransform = m_TargetAtt->Model()->m_Transform;

	GameObject* _newRoot = GameObject::Spawn(m_TargetAtt->GetGameObject()->GetScene());
	Quaternion _newRootRotation = Quaternion::LookRotation(GetGameObject()->m_Transform.GetForwardDirection() * -1, Vector3::Up);
	_newRoot->m_Transform.SetWorldTM(
		Matrix::CreateScale(Vector3::One) 
		* Matrix::CreateFromQuaternion(_newRootRotation) 
		* Matrix::CreateTranslation(_targetTransform.GetGameObject()->m_Transform.GetWorldPosition()));
	_targetTransform.GetGameObject()->SetParent(_newRoot);
	_newRoot->m_Transform.SetWorldScale(Vector3{ 1, 1, 0.1f });

	//m_OriginScl = _targetTransform.GetWorldScale();

	//auto _objectWorldTm = m_ObjT->GetWorldTM();
	//auto _playerWorldTm = m_MyT->GetWorldTM();

	//// Determine the direction of the player's view
	//auto playerDirection = m_MyT->GetForwardDirection();
	//playerDirection.Normalize();

	//// Get the scale, rotation, and position of the object relative to the player's view
	//Vector3 _objectScale, _objectPosition;
	//Quaternion _objectRotation;
	//TL_Math::Matrix _relativeTm = _objectWorldTm * _playerWorldTm.Invert();
	//_relativeTm.Decompose(_objectScale, _objectRotation, _objectPosition);

	//// Remove the component of the scale along the player's view direction
	//_objectScale -= playerDirection * playerDirection.Dot(_objectScale);

	//// Recompose the relative transformation matrix with the updated scale
	//_relativeTm =
	//	Matrix::CreateFromQuaternion(_objectRotation) *
	//	Matrix::CreateScale(_objectScale) *
	//	Matrix::CreateTranslation(_objectPosition);

	//auto _newTM = _relativeTm * _playerWorldTm;
	//// Update the world transformation matrix of the target object
	//_targetTransform.SetWorldTM(_newTM);

	//auto sd = _targetTransform.GetWorldRotation();

	//auto _modt = m_TargetAtt->Model()->m_Transform;
	//m_OriginLoc = _modt.GetLocalPosition();
	//m_OriginScl = _modt.GetLocalScale();
	//m_OriginRot = _modt.GetLocalRotation();

	//auto _ObjM = m_ObjT->GetWorldTM();
	//
	//auto _pCamM = m_MyT->GetWorldTM();
	//_pCamM.m[3][0] = _impactPoint.x;
	//_pCamM.m[3][1] = _impactPoint.y;
	//_pCamM.m[3][2] = _impactPoint.z;
	//TL_Math::Matrix _invpCam;
	//_pCamM.Invert(_invpCam);

	//auto _multTM = _ObjM * _invpCam;

	//Vector3 _mScl;
	//Quaternion _mRot;
	//Vector3 _mPos;

	//_multTM.Decompose(_mScl, _mRot, _mPos);

	//Vector3 _newScl = {_mScl.x, _mScl.y, 0.0001f};

	//Matrix _tmpTM =
	//	Matrix::CreateFromQuaternion(_mRot)
	//	* Matrix::CreateScale(_newScl)
	//	* Matrix::CreateTranslation(_mPos);

	//_tmpTM = _tmpTM * _pCamM;
	//
	//m_TargetAtt->Model()->m_Transform.SetWorldTM(_tmpTM);
}
