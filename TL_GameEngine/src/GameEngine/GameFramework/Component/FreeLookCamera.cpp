#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/FreeLookCamera.h"

#include "GameEngine/Gameframework/GameObject.h"

#include "Application/GameTime.h"
#include "Application/Input.h"
#include "Log/Log.h"

#include "rttr/registration"
RTTR_REGISTRATION
{
    using namespace TL_GameEngine;

    rttr::registration::class_<FreeLookCamera>("TL_GameEngine::FreeLookCamera")
        .constructor<GameObject*, const tstring&>()
        .property("m_Speed", &FreeLookCamera::m_Speed, rttr::registration::private_access);
}

namespace TL_GameEngine
{
    using namespace TL_Application;
    using namespace TL_Math;

    void FreeLookCamera::Tick()
    {
        ComponentBase::Tick();
        ControlCamera();
    }

    void FreeLookCamera::ControlCamera()
    {
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

            // // (deprecated) Matrix로 회전합니다.
            // if (_rotationPitch != 0)
            //     PitchUsingMatrix(_rotationPitch, _rightDir, _upDir, _forwardDir);
            // if (_rotationY != 0)
            //     RotateYUsingMatrix(_rotationY, _rightDir, _upDir, _forwardDir);

            // Quaternion으로 회전합니다.
            if (_rotationPitch != 0)
                _rotation = _rotation * Quaternion::CreateFromAxisAngle(_rightDir, _rotationPitch);
            if (_rotationY != 0)
                _rotation = _rotation * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotationY);

            _rightDir = Vector3::Transform(Vector3::Right, _rotation);
            _upDir = Vector3::Transform(Vector3::Up, _rotation);
            _forwardDir = Vector3::Transform(Vector3::Forward, _rotation);
        }

        // 키보드로 위치를 변경합니다.

        float _forward = 0;
        float _right = 0;
        float _up = 0;

        if (Input::GetKeyPress(EVirtualKey::Key_U))
            _forward += 1.f;
        if (Input::GetKeyPress(EVirtualKey::Key_J))
            _forward -= 1.f;

        if (Input::GetKeyPress(EVirtualKey::Key_K))
            _right += 1.f;
        if (Input::GetKeyPress(EVirtualKey::Key_H))
            _right -= 1.f;

        if (Input::GetKeyPress(EVirtualKey::Key_I))
            _up += 1.f;
        if (Input::GetKeyPress(EVirtualKey::Key_Y))
            _up -= 1.f;

        if (_forward != 0 || _right != 0 || _up != 0
            || _rotationPitch != 0 || _rotationY != 0)
        {
            auto _direction = (_forwardDir * _forward + _rightDir * _right + _upDir * _up);
            _direction.Normalize();

            const auto _nextPosition = _transform->GetWorldPosition() + _direction * m_Speed * GameTime::GetDeltaTime();

            _transform->SetWorldTM(Matrix{
                _rightDir.x, _rightDir.y, _rightDir.z, 0.f,
                _upDir.x, _upDir.y, _upDir.z, 0.f,
                _forwardDir.x, _forwardDir.y, _forwardDir.z, 0.f,
                _nextPosition.x, _nextPosition.y, _nextPosition.z, 1.f
            });
        }
    }

    void FreeLookCamera::PitchUsingMatrix(float _angle, Vector3 _right, Vector3& _up, Vector3& _forward) const
    {
        const Matrix _rotationMatrix = Matrix::CreateFromAxisAngle(_right, _angle);

        Vector3::TransformNormal(_up, _rotationMatrix, _up);
        Vector3::TransformNormal(_forward, _rotationMatrix, _forward);
    }

    void FreeLookCamera::RotateYUsingMatrix(float _angle, Vector3& _right, Vector3& _up, Vector3& _forward) const
    {
        const Matrix _rotationMatrix = Matrix::CreateRotationY(_angle);

        Vector3::TransformNormal(_right, _rotationMatrix, _right);
        Vector3::TransformNormal(_up, _rotationMatrix, _up);
        Vector3::TransformNormal(_forward, _rotationMatrix, _forward);
    }
}
