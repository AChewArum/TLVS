#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

namespace TL_GameEngine
{
	class Transform;
}

class ObjectControl :
	public TL_GameEngine::ComponentBase
{
public:
	ObjectControl(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("ObjectControl"))
		: ComponentBase(_gameObject, _typeName) {}

	void PreTick() override;

	void Tick() override;
	void FixedTick() override;
	void OnStart() override;

	TL_GameEngine::GameObject* GetGrabTarget();

	void SetGrab();

	void SetGrabDistance(bool _plus);

	//무언가가 들어가긴 해야할텐데
	void RotateObject(TL_Math::Vector3 _dir);

	void AttachObject(TL_GameEngine::GameObject* _target);
	void DetachObject();
	void MoveObject();

	TL_GameEngine::Transform* PlayerCameraTransform() const { return m_PlayerCameraTransform; }
	void PlayerCameraTransform(TL_GameEngine::Transform* _playerCameraTransform) { m_PlayerCameraTransform = _playerCameraTransform; }

private:
	float m_Distance = 10.0f;
	float m_MinDistance = 1.0f;
	float m_MaxDistance = 15.0f;
	float m_Delta = 0.05f;

	TL_GameEngine::GameObject* m_GrabbedObject = nullptr;

	TL_GameEngine::Transform* m_GrabbedObjectTransform = nullptr;
	TL_GameEngine::Transform* m_PlayerCameraTransform = nullptr;

	bool m_IsGrabOn = false;
	bool m_IsRotateOn = false;
	bool m_IsMovable = false;
	const int32 m_GrabChannel = 0b10000;

	float m_TargetLength = 20;
	TL_Math::Vector3 m_TargetLoc = {0, 0, 0};

	

};
