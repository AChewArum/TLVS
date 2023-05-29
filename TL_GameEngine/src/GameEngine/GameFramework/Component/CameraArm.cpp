#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/CameraArm.h"

#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/FreeLookCamera.h"

#include "Application/Input.h"

namespace TL_GameEngine
{
    using namespace TL_Application;

    void CameraArm::Tick()
    {
        ComponentBase::Tick();

        const auto& _transform = GetGameObject()->GetComponent<Transform>();

        Vector3 _rightDir = _transform->GetRightDirection();
        Vector3 _upDir = _transform->GetUpDirection();
        Vector3 _forwardDir = _transform->GetForwardDirection();
        Quaternion _rotation = _transform->GetWorldRotation();

        // 마우스로 회전합니다.

        float _rotationPitch = 0; 
        float _rotationY = 0;

        if (Input::GetKeyPress(EVirtualKey::MouseRight))
        {
            const auto _mouseDelta = Input::GetMouseMovementDelta();
            _rotationPitch = _mouseDelta.y * 0.1f * DEG_TO_RAD;
            _rotationY = _mouseDelta.x * 0.1f * DEG_TO_RAD;

            // Quaternion으로 회전합니다.
            if (_rotationPitch != 0)
                _rotation = _rotation * Quaternion::CreateFromAxisAngle(_rightDir, _rotationPitch);
            if (_rotationY != 0)
                _rotation = _rotation * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotationY);

            if (_rotationPitch != 0 || _rotationY != 0)
            {
                _rightDir = Vector3::Transform(Vector3::Right, _rotation);
                _upDir = Vector3::Transform(Vector3::Up, _rotation);
                _forwardDir = Vector3::Transform(Vector3::Forward, _rotation);

                _transform->SetLocalRotation(Quaternion::CreateFromRotationMatrix(
                    Matrix{
                        _rightDir.x, _rightDir.y, _rightDir.z, 0.f,
                        _upDir.x, _upDir.y, _upDir.z, 0.f,
                        _forwardDir.x, _forwardDir.y, _forwardDir.z, 0.f,
                        0, 0, 0, 1.f
                    }
                ));
            }
        }

        auto _targetPosition = m_Target->GetComponent<Transform>()->GetWorldPosition() + m_Offset;
        auto _lerpedPosition = Vector3::Lerp(_transform->GetWorldPosition(), _targetPosition, 0.1f);
        _transform->SetWorldPosition(_lerpedPosition);
    }
}
