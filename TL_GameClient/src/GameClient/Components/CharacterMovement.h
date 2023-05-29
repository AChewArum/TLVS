#pragma once
#include "Movement.h"

class StateMachine;

namespace TL_GameEngine
{
	class Animator;
	class RigidBody;
	class Transform;
}

class CharacterMovement :
	public Movement
{
public:
	CharacterMovement(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("CharacterMovement"))
		: Movement(_gameObject, _typeName), m_Player(nullptr), m_IsRunning(false) {}

	void OnStart() override;
	void PreTick() override;

	void Tick() override;
	void FixedTick() override;
	void PostTick() override;

	void Move(float _dir);
	void Stop();
	void Jump();
	void GetCamYawQuaternion(TL_Math::Quaternion& _camYaw) const;
	void Rotate();

	bool IsRunning() const { return m_IsRunning; }
	void IsRunning(bool _isRunning) { m_IsRunning = _isRunning; }

	TL_GameEngine::Transform* PlayerCameraTransform() const { return m_PlayerCameraTransform; }
	void PlayerCameraTransform(TL_GameEngine::Transform* _playerCameraTransform) { m_PlayerCameraTransform = _playerCameraTransform; }

	TL_GameEngine::GameObject* PlayerModel() const { return m_PlayerModel; }
	void PlayerModel(TL_GameEngine::GameObject* _playerModel) { m_PlayerModel = _playerModel; }

	bool IsCharRotate() const { return m_IsCharRotate; }
	void IsCharRotate(bool _isCharRotate) { m_IsCharRotate = _isCharRotate; }

	void CheckCollision();

private:
	//Á¡ÇÁ·Â
	float m_JumpPower = 15.0f;
	TL_GameEngine::GameObject* m_Player;
	TL_GameEngine::GameObject* m_PlayerModel = nullptr;
	TL_Math::Vector3 CalcDirection(float _dir);
	TL_GameEngine::Transform* m_PlayerTransform = nullptr;
	TL_GameEngine::Transform* m_PlayerCameraTransform = nullptr;
	TL_GameEngine::RigidBody* m_PlayerRigidBody = nullptr;
	TL_GameEngine::Animator* m_Animator = nullptr;
	StateMachine* m_PlayerState = nullptr;
	float m_RunningVal = 3.0f;
	bool m_IsRunning;
	bool m_IsCharRotate = true;
};
