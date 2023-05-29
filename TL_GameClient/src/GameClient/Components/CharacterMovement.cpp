#include "CharacterMovement.h"

#include "AnimationController.h"
#include "StateMachine.h"
#include "GameEngine/GameFramework/Debug.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/Gameframework/Component/Animator.h"
#include "GameEngine/GameFramework/Component/Collider.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/GameFramework/Component/RigidBody.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/GameFramework/Physics/RaycastHit.h"
#include "Log/Log.h"

using namespace TL_Math;
using namespace TL_GameEngine;

void CharacterMovement::OnStart()
{
	ComponentBase::OnStart();
	m_Player = m_GameObject;
	m_PlayerTransform = m_Player->GetComponent<Transform>();

	m_PlayerCameraTransform = m_GameObject->GetChild(TEXT("CamTransform"))->GetComponent<TL_GameEngine::Transform>();

	m_PlayerRigidBody = m_GameObject->GetComponent<RigidBody>();
	if (m_PlayerState = m_Player->GetComponent<StateMachine>()) {}
	if (m_PlayerModel->GetChild(L"Armature") != nullptr)
	{
		m_Animator = m_PlayerModel->GetChild(L"Armature")->GetComponent<Animator>();
	}
	Speed(20.0f);
}

void CharacterMovement::PreTick()
{
	Movement::PreTick();
}

void CharacterMovement::Tick()
{
	ComponentBase::Tick();
}

void CharacterMovement::FixedTick()
{
	Movement::FixedTick();
}

void CharacterMovement::PostTick()
{
	Movement::PostTick();
}

void CharacterMovement::Move(float _dir)
{
	if (m_PlayerRigidBody == nullptr) return;
	Vector3 _vDir;
	_vDir = CalcDirection(_dir) * m_Speed;
	//if(m_PlayerState->GetCurState()==eFSMSTATE::JUMP)
	//{
	auto _gravity = m_PlayerRigidBody->GetLinearVelocity().y;
	_vDir.y = _gravity;
	//}

	m_PlayerRigidBody->SetLinearVelocity(_vDir);
}

void CharacterMovement::Stop()
{
	if (m_PlayerRigidBody == nullptr) return;
	Vector3 _vDir = {0, 0, 0};
	auto _gravity = m_PlayerRigidBody->GetLinearVelocity().y;
	_vDir.y = _gravity;
	m_PlayerRigidBody->SetLinearVelocity(_vDir);
}

void CharacterMovement::Jump()
{
	if (m_PlayerRigidBody == nullptr) return;
	const auto _curState = m_PlayerState->GetCurState();

	if (_curState != eFSMSTATE::JUMP)
	{
		m_PlayerRigidBody->AddForce(Vector3::Up * 10.0f, eForceMode::VelocityChange);
	}
}

void CharacterMovement::GetCamYawQuaternion(Quaternion& _camYaw) const
{
	auto _camrot = m_PlayerCameraTransform->GetLocalRotation();
	auto _camroteuler = _camrot.ToEuler();
	_camYaw = Quaternion::CreateFromAxisAngle(Vector3::Up, _camroteuler.y);
}

void CharacterMovement::Rotate()
{
	Quaternion _newRotation;
	GetCamYawQuaternion(_newRotation);

	m_PlayerModel->m_Transform.SetWorldRotation(_newRotation);
}

void CharacterMovement::CheckCollision()
{
	Vector3 _dir = m_PlayerRigidBody->GetLinearVelocity();
	_dir.y += 0.01;
	_dir.Normalize();
	Vector3 _origin = m_PlayerTransform->GetWorldPosition();
	RaycastHit _t;

	if (PhysicsSystem::Instance().Raycast(_origin, _dir, _t, 1.0f, 0b10))
	{
		Vector3 _vDir;
		auto _curdir = m_PlayerRigidBody->GetLinearVelocity();
		auto _finaldir = m_PlayerRigidBody->GetLinearVelocity() - _curdir;
		if (m_PlayerState->GetCurState() == eFSMSTATE::JUMP)
		{
			_finaldir.y = _curdir.y;
		}

		m_PlayerRigidBody->SetLinearVelocity(_finaldir);
		Log::Client_Debug("Blocked");
	}
}

TL_Math::Vector3 CharacterMovement::CalcDirection(float _dir)
{
	//입력받은 방향의 쿼터니언 생성
	auto _q = Quaternion::CreateFromAxisAngle(Vector3::Up, _dir);

	Quaternion _camYaw;
	GetCamYawQuaternion(_camYaw);

	Vector3 _v = Vector3::Transform(Vector3::Forward, _camYaw);;

	//카메라 방향벡터에 입력받은 회전값만큼 회전
	_v = Vector3::Transform(_v, _q);

	_v.Normalize(_v);
	return _v;
}
