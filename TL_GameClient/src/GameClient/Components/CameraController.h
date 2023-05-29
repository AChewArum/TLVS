#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

namespace TL_GameEngine
{
	class Camera;
	class Transform;
}

class CameraController :
	public TL_GameEngine::ComponentBase
{
public:
	CameraController(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("CameraController"))
		: ComponentBase(_gameObject, _typeName) {}

	void Tick() override;
	void OnStart() override;

	void Rotate();

	TL_GameEngine::Transform* Camera() const { return m_CameraTransform; }
	void Camera(TL_GameEngine::Transform* _camera) { m_CameraTransform = _camera; }

private:
	TL_GameEngine::Transform* m_CameraTransform = nullptr;
	float m_RotationDelta = 0.3f;
};
