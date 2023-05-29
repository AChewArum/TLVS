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
            // 애니메이션을 재생한지 얼마 되지 않아
            // end 키프레임이 애니메이션의 첫 키프레임을 가리킬 때 실행됩니다.
            // begin 키프레임을 애니메이션의 마지막 프레임으로 설정합니다.

            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else if (_keyFrameEndIter == _keyFrameList.end())
        {
            // position 애니메이션의 가장 마지막 프레임 이후에서
            // max frame 사이를 재생하고 있는 경우 실행됩니다.

            _keyFrameEndIter = _keyFrameList.begin();
            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else
        {
            _keyframeStartIter = _keyFrameEndIter - 1;
        }

        // 두 키프레임간 값을 보간합니다.

        const float _keyframeStartTime = _keyframeStartIter->time;
        const float _keyframeEndTime = _keyFrameEndIter->time;

        float _lengthFromStartToEnd;
        float _lengthFromStartToTarget;

        if (_keyframeEndTime < _keyframeStartTime)
        {
            // 애니메이션 클립의 가장 마지막 프레임을 start로 재생하고 있어
            // 클립의 시작 프레임과 보간될 때 실행됩니다.

            _lengthFromStartToEnd = (_clipLength - _keyframeStartTime) + _keyframeEndTime;

            if (_playingTime < _keyframeEndTime)
            {
                // 재생 시간이 [0, 프레임 종료 시간] 사이 구간에 있는 경우 실행됩니다.
                _lengthFromStartToTarget = (_clipLength - _keyframeStartTime) + _playingTime;
            }
            else
            {
                // 재생 시간이 [프레임 시작 시간, 클립의 길이] 사이 구간에 있는 경우 실행됩니다.
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
            // 애니메이션을 재생한지 얼마 되지 않아
            // end 키프레임이 애니메이션의 첫 키프레임을 가리킬 때 실행됩니다.
            // begin 키프레임을 애니메이션의 마지막 프레임으로 설정합니다.

            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else if (_keyFrameEndIter == _keyFrameList.end())
        {
            // position 애니메이션의 가장 마지막 프레임 이후에서
            // max frame 사이를 재생하고 있는 경우 실행됩니다.

            _keyFrameEndIter = _keyFrameList.begin();
            _keyframeStartIter = _keyFrameList.end() - 1;
        }
        else
        {
            _keyframeStartIter = _keyFrameEndIter - 1;
        }

        // 두 키프레임간 값을 보간합니다.

        const float _keyframeStartTime = _keyframeStartIter->time;
        const float _keyframeEndTime = _keyFrameEndIter->time;

        float _lengthFromStartToEnd;
        float _lengthFromStartToTarget;

        if (_keyframeEndTime < _keyframeStartTime)
        {
            // 애니메이션 클립의 가장 마지막 프레임을 start로 재생하고 있어
            // 클립의 시작 프레임과 보간될 때 실행됩니다.

            _lengthFromStartToEnd = (_clipLength - _keyframeStartTime) + _keyframeEndTime;

            if (_playingTime < _keyframeEndTime)
            {
                // 재생 시간이 [0, 프레임 종료 시간] 사이 구간에 있는 경우 실행됩니다.
                _lengthFromStartToTarget = (_clipLength - _keyframeStartTime) + _playingTime;
            }
            else
            {
                // 재생 시간이 [프레임 시작 시간, 클립의 길이] 사이 구간에 있는 경우 실행됩니다.
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
