#pragma once

#include "Common.h"

#include "Math\TL_Math.h"

#include "GameEngine\GameFramework\Component\ComponentBase.h"
#include "GameEngine/Resource/Resource.h"

// �ִϸ��̼��� ���, ���� ���� ����
// State �б� ���� ����
// State �� ���� �ִϸ��̼� ����.

namespace TL_GameEngine
{
	class AnimationClip;

	class GAME_ENGINE_API Animator
		: public ComponentBase
	{
	public:
		Animator(class GameObject* _gameObject, const tstring& _typeName = TEXT("Animator"));
		virtual ~Animator();

		//////////////////////////////////////////////////////////////////////////
		// Inherited Component
		//////////////////////////////////////////////////////////////////////////

		void OnStart() override;

		void Tick() override;

		virtual void PreTick();
		bool AnimatorTick();

		virtual void PostTick();

		virtual void OnChangeEnable(bool _newEnable);

	protected:
		ComponentBase* CreateInstance_Impl(GameObject* _owner) override;
		ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;
		void CloneConvertAddress_Impl(AddressConvertMap* _map) override;

	public:
		/// @brief ���� �̸��� �´� Ŭ���� ������ ���� ��� �ִϸ��̼��� ���� ���� �ش�.
		/// @param name Ŭ�� �̸�
		void Play(const tstring& name);

		/// @brief �ִϸ��̼��� ���߰� ��� Ÿ�ٿ�����Ʈ�� �ʱ� ���·� �ǵ��� �ش�.
		void Stop();

		bool GetIsPlaying() const { return m_IsPlaying; }

		float GetPlayingTime() const { return m_PlayingTime; }

		std::vector<Resource<AnimationClip>> GetAnimationClipList() const { return m_AnimationClipList; }

		Resource<AnimationClip>& GetAnimationClip(uint32 index);
		Resource<AnimationClip>& GetAnimationClip(const tstring& clipName);
		void SetClip(Resource<AnimationClip> _clip) { m_CurrAnimationClip = _clip; }

		void AddClip(Resource<AnimationClip>& clip);
		void RemoveClip(Resource<AnimationClip>& clip);
		void RemoveClip(const tstring& name);

		void InitializeTargetList();
		void InitializeTargetList(Resource<AnimationClip>& _clip);

		void StartTransition(tstring _animName);

		bool IsBlend();

		float CalculateBlendRatio();

	private:
		/// @brief �ִϸ��̼��� �����ϱ����� ���ӿ�����Ʈ�� ����Ʈ
		/// �ִϸ��̼��� ����Ǿ���� ���� ������Ʈ�� �����Ǵ� ��찡 ������ ����Ǿ�� �Ѵ�.
		/// ���� ������� ���Ѵٸ� �� ������ �ִϸ��̼��� ����Ǿ���� ���ӿ�����Ʈ�� ã�ƾ� �Ѵ�.
		struct TargetList
		{
			bool bInit = false;
			std::vector<GameObject*> targets;
			std::vector<Matrix> initialLocalTMs;
		};

		/**
		 * @brief ���� ������� Ŭ���� ��� ��ġ�Դϴ�.
		 * �� ���� �׻� Ŭ���� ���̺��� ª���ϴ�.
		*/
		float m_PlayingTime;

		float m_PrePlayingTime; // ������ �� ���� �ִϸ��̼��� Time

		/// @brief �ִϸ��̼��� ���࿩��
		bool m_IsPlaying;

		/// @brief ���� ���� ���� Ŭ�� ĳ��
		Resource<AnimationClip> m_CurrAnimationClip;
		TargetList m_CurrTarget;
		uint32 m_CurrIndex;

		Resource<AnimationClip> m_PreAnimationClip;
		uint32 m_PreIndex;

		std::vector<Resource<AnimationClip>> m_AnimationClipList;

		// animation Transition ������

		bool m_IsTrasition;

		float m_Duration;

		float m_BlendStartTime;
	};
}

