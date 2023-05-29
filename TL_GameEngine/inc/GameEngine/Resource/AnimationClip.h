#pragma once

#include "Common.h"
#include "Math/TL_Math.h"

namespace TL_GameEngine
{
	/**
	* \brief 게임 오브젝트의 움직임의 한 프레임
	*/
	struct Keyframe_Vector3
	{
		float time;
		TL_Math::Vector3 data;
	};

	struct Keyframe_Quaternion
	{
		float time;
		TL_Math::Vector4 data;
	};

	/**
	* \brief 한 게임 오브젝트의 움직임
	*/
	class AnimationSnap
	{
	public:
		void AddPositionKeyFrame(const Keyframe_Vector3& _keyframe) { m_PosKeyFrameList.push_back(_keyframe); }
		void AddRotationKeyFrame(const Keyframe_Quaternion& _keyframe) { m_RotKeyFrameList.push_back(_keyframe); }
		void AddScaleKeyFrame(const Keyframe_Vector3& _keyframe) { m_ScaleKeyFrameList.push_back(_keyframe); }
		
		/**
		 * \brief 애니메이션 초기화시 타겟될 게임 오브젝트를 찾기 위한 값 게임 오브젝트의 이름과 같은 값을 타겟 해준다.
		 */
		tstring m_TargetName;

		/**
		 * \brief 이 스냅이 담고있는 애니메이션의 최대 길이 값
		 */
		float m_MaxFrameRate = 0;

		/**
		 * \brief Transform Animation 를 담고 있는 컨테이너
		 */
		std::vector<Keyframe_Vector3> m_PosKeyFrameList;
		std::vector<Keyframe_Quaternion> m_RotKeyFrameList;;
		std::vector<Keyframe_Vector3> m_ScaleKeyFrameList;
	};

	class AnimationClip 
	{
	public:
		AnimationClip(const tstring& clipName, std::vector<AnimationSnap>&& snapList)
			: m_ClipName(clipName)
			, m_AnimationSnapList(std::move(snapList))
		{
			// ClipLength를 초기화합니다.

			m_ClipLength = 0;
			for(const auto& _snap : m_AnimationSnapList)
			{
				if (m_ClipLength < _snap.m_MaxFrameRate)
					m_ClipLength = _snap.m_MaxFrameRate;
			}
		}

	    // inline bool operator==(const tstring& value);
		
		const tstring& GetClipName() const { return m_ClipName; }
		void SetClipName(const tstring& value) { m_ClipName = value; }

		const AnimationSnap& GetAnimationSnap(uint32 index) { assert(index < m_AnimationSnapList.size());  return m_AnimationSnapList[index]; }
		const std::vector<AnimationSnap>& GetAnimationSnapList() const { return m_AnimationSnapList; }

		float GetClipLength() const { return m_ClipLength; }
		
		static TL_Math::Vector3 GetLerpedVector3(
            const std::vector<Keyframe_Vector3>& _keyFrameList,
            float _clipLength, 
			float _playingTime);

		static TL_Math::Quaternion GetSlerpedQuaternion(
			const std::vector<Keyframe_Quaternion>& _keyFrameList,
			float _clipLength,
			float _playingTime);

	private:
		/**
		 * \brief frame 에 담긴 frameRate에 따라 이전 프레임과 다음 프레임 사이를 보간 시켜 gameObject에 적용 시켜준다.
		 * @param m_ClipName 크립의 이름
		 */
		tstring m_ClipName;

		// 게임 오브젝트 갯수 만큼 스냅 데이터가 있음
		std::vector<AnimationSnap> m_AnimationSnapList;

		/**
		 * @brief 애니메이션 클립의 길이입니다.
		 * 이 값은 이 클립이 포함하는 모든 AnimationSnap이 가지고 있는 MaxFrameRate중 최댓값으로 초기화됩니다.
		*/
		float m_ClipLength;
	};
}
