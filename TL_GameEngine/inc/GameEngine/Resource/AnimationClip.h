#pragma once

#include "Common.h"
#include "Math/TL_Math.h"

namespace TL_GameEngine
{
	/**
	* \brief ���� ������Ʈ�� �������� �� ������
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
	* \brief �� ���� ������Ʈ�� ������
	*/
	class AnimationSnap
	{
	public:
		void AddPositionKeyFrame(const Keyframe_Vector3& _keyframe) { m_PosKeyFrameList.push_back(_keyframe); }
		void AddRotationKeyFrame(const Keyframe_Quaternion& _keyframe) { m_RotKeyFrameList.push_back(_keyframe); }
		void AddScaleKeyFrame(const Keyframe_Vector3& _keyframe) { m_ScaleKeyFrameList.push_back(_keyframe); }
		
		/**
		 * \brief �ִϸ��̼� �ʱ�ȭ�� Ÿ�ٵ� ���� ������Ʈ�� ã�� ���� �� ���� ������Ʈ�� �̸��� ���� ���� Ÿ�� ���ش�.
		 */
		tstring m_TargetName;

		/**
		 * \brief �� ������ ����ִ� �ִϸ��̼��� �ִ� ���� ��
		 */
		float m_MaxFrameRate = 0;

		/**
		 * \brief Transform Animation �� ��� �ִ� �����̳�
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
			// ClipLength�� �ʱ�ȭ�մϴ�.

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
		 * \brief frame �� ��� frameRate�� ���� ���� �����Ӱ� ���� ������ ���̸� ���� ���� gameObject�� ���� �����ش�.
		 * @param m_ClipName ũ���� �̸�
		 */
		tstring m_ClipName;

		// ���� ������Ʈ ���� ��ŭ ���� �����Ͱ� ����
		std::vector<AnimationSnap> m_AnimationSnapList;

		/**
		 * @brief �ִϸ��̼� Ŭ���� �����Դϴ�.
		 * �� ���� �� Ŭ���� �����ϴ� ��� AnimationSnap�� ������ �ִ� MaxFrameRate�� �ִ����� �ʱ�ȭ�˴ϴ�.
		*/
		float m_ClipLength;
	};
}
