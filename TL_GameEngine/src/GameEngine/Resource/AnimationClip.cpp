#include "GameEngine_pch.h"
#include "GameEngine/Resource/AnimationClip.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Transform.h"

namespace TL_GameEngine
{
    // bool AnimationClip::operator==(const tstring& value)
    // {
    //     return (m_ClipName == value);
    // }

    TL_Math::Vector3 AnimationClip::GetLerpedVector3(
        const std::vector<Keyframe_Vector3>& _keyFrameList,
        float _clipLength,
        float _playingTime
    )
    {
        auto _keyFrameEndIter = std::lower_bound(
            _keyFrameList.begin(), _keyFrameList.end(),
            _playingTime,
            [](const Keyframe_Vector3& _keyframe, float _targetTime)
            {
                return _keyframe.time <= _targetTime;
            }
        );

        decltype(_keyFrameEndIter) _keyframeStartIter;

        if (_keyFrameEndIter == _keyFrameList.begin())
        {
            // �ִϸ��̼��� ������� �� ���� �ʾ�
            // end Ű�������� �ִϸ��̼��� ù Ű�������� ����ų �� ����˴ϴ�.
            // begin Ű�������� �ִϸ��̼��� ������ ���������� �����մϴ�.

            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else if (_keyFrameEndIter == _keyFrameList.end())
        {
            // position �ִϸ��̼��� ���� ������ ������ ���Ŀ���
            // max frame ���̸� ����ϰ� �ִ� ��� ����˴ϴ�.

            _keyFrameEndIter = _keyFrameList.begin();
            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else
        {
            _keyframeStartIter = _keyFrameEndIter - 1;
        }

        // �� Ű�����Ӱ� ���� �����մϴ�.

        const float _keyframeStartTime = _keyframeStartIter->time;
        const float _keyframeEndTime = _keyFrameEndIter->time;

        float _lengthFromStartToEnd;
        float _lengthFromStartToTarget;

        if (_keyframeEndTime < _keyframeStartTime)
        {
            // �ִϸ��̼� Ŭ���� ���� ������ �������� start�� ����ϰ� �־�
            // Ŭ���� ���� �����Ӱ� ������ �� ����˴ϴ�.

            _lengthFromStartToEnd = (_clipLength - _keyframeStartTime) + _keyframeEndTime;

            if (_playingTime < _keyframeEndTime)
            {
                // ��� �ð��� [0, ������ ���� �ð�] ���� ������ �ִ� ��� ����˴ϴ�.
                _lengthFromStartToTarget = (_clipLength - _keyframeStartTime) + _playingTime;
            }
            else
            {
                // ��� �ð��� [������ ���� �ð�, Ŭ���� ����] ���� ������ �ִ� ��� ����˴ϴ�.
                _lengthFromStartToTarget = _playingTime - _keyframeStartTime;
            }
        }
        else
        {
            _lengthFromStartToEnd = _keyframeEndTime - _keyframeStartTime;
            _lengthFromStartToTarget = _playingTime - _keyframeStartTime;
        }

        const float _delta = _lengthFromStartToTarget / _lengthFromStartToEnd;
        const Vector3 _value = Vector3::Lerp(_keyframeStartIter->data, _keyFrameEndIter->data, _delta);
        return _value;
    }

    TL_Math::Quaternion AnimationClip::GetSlerpedQuaternion(
        const std::vector<Keyframe_Quaternion>& _keyFrameList,
        float _clipLength,
        float _playingTime
    )
    {
        auto _keyFrameEndIter = std::lower_bound(
            _keyFrameList.begin(), _keyFrameList.end(),
            _playingTime,
            [](const Keyframe_Quaternion& _keyframe, float _targetTime)
            {
                return _keyframe.time <= _targetTime;
            }
        );

        decltype(_keyFrameEndIter) _keyframeStartIter;

        if (_keyFrameEndIter == _keyFrameList.begin())
        {
            // �ִϸ��̼��� ������� �� ���� �ʾ�
            // end Ű�������� �ִϸ��̼��� ù Ű�������� ����ų �� ����˴ϴ�.
            // begin Ű�������� �ִϸ��̼��� ������ ���������� �����մϴ�.

            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else if (_keyFrameEndIter == _keyFrameList.end())
        {
            // position �ִϸ��̼��� ���� ������ ������ ���Ŀ���
            // max frame ���̸� ����ϰ� �ִ� ��� ����˴ϴ�.

            _keyFrameEndIter = _keyFrameList.begin();
            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else
        {
            _keyframeStartIter = _keyFrameEndIter - 1;
        }

        // �� Ű�����Ӱ� ���� �����մϴ�.

        const float _keyframeStartTime = _keyframeStartIter->time;
        const float _keyframeEndTime = _keyFrameEndIter->time;

        float _lengthFromStartToEnd;
        float _lengthFromStartToTarget;

        if (_keyframeEndTime < _keyframeStartTime)
        {
            // �ִϸ��̼� Ŭ���� ���� ������ �������� start�� ����ϰ� �־�
            // Ŭ���� ���� �����Ӱ� ������ �� ����˴ϴ�.

            _lengthFromStartToEnd = (_clipLength - _keyframeStartTime) + _keyframeEndTime;

            if (_playingTime < _keyframeEndTime)
            {
                // ��� �ð��� [0, ������ ���� �ð�] ���� ������ �ִ� ��� ����˴ϴ�.
                _lengthFromStartToTarget = (_clipLength - _keyframeStartTime) + _playingTime;
            }
            else
            {
                // ��� �ð��� [������ ���� �ð�, Ŭ���� ����] ���� ������ �ִ� ��� ����˴ϴ�.
                _lengthFromStartToTarget = _playingTime - _keyframeStartTime;
            }
        }
        else
        {
            _lengthFromStartToEnd = _keyframeEndTime - _keyframeStartTime;
            _lengthFromStartToTarget = _playingTime - _keyframeStartTime;
        }

        const float _delta = _lengthFromStartToTarget / _lengthFromStartToEnd;
        const Quaternion _value = Quaternion::Slerp(_keyframeStartIter->data, _keyFrameEndIter->data, _delta);
        return _value;
    }
}
