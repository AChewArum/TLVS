#include "CameraController.h"

#include "Application/Input.h"
#include "GameEngine/GameFramework/Debug.h"
#include "GameEngine/GameFramework/Component/Camera.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "Math/TL_Math.h"
using namespace TL_Math;

void CameraController::OnStart()
{
	ComponentBase::OnStart();
	m_CameraTransform = m_GameObject->GetChild(TEXT("CamTransform"))->GetComponent<TL_GameEngine::Transform>();
}

void CameraController::Rotate()
{
	//ī�޶��� ������ �� ������ ������
	Vector3 _rightDir = m_CameraTransform->GetRightDirection();

	//�����̼� �ʱ�ȭ
	float _rotPitch = 0;
	float _rotY = 0;
	TL_Math::Quaternion _rot = Quaternion::Identity;

	//���콺�� ��Ÿ �� ��������
	const auto _mouseDelta = TL_Application::Input::GetMouseMovementDelta();
	_rotPitch = _mouseDelta.y * m_RotationDelta * 0.01745329252f;
	_rotY = _mouseDelta.x * m_RotationDelta * 0.01745329252f;


	//��Ÿ���� ������ �԰��� ȸ��
	if (_rotPitch != 0)
	{
		_rot = Quaternion::CreateFromAxisAngle(_rightDir, _rotPitch);
	}
	if (_rotY != 0)
	{
		_rot = _rot * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotY);
	}

	if (_rotPitch != 0 || _rotY != 0)
	{
		auto _cameraRot = m_CameraTransform->GetLocalRotation();


		Quaternion _targetRot = m_CameraTransform->GetLocalRotation() * _rot;
		Vector3 _clampedRotEuler = _targetRot.ToEuler();
		const float _clampedRotX = std::clamp(_clampedRotEuler.x, -1.2f, 1.2f);

		_clampedRotEuler.x = _clampedRotX;

		auto _newRotation = Quaternion::CreateFromYawPitchRoll(_clampedRotEuler.y, _clampedRotEuler.x, 0);


		m_CameraTransform->SetLocalRotation(_newRotation);
	}
}



void CameraController::Tick()
{
	ComponentBase::Tick();
}
