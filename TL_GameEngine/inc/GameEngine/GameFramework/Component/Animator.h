#pragma once

#include "Common.h"

#include "Math\TL_Math.h"

#include "GameEngine\GameFramework\Component\ComponentBase.h"
#include "GameEngine/Resource/Resource.h"

// 애니메이션을 재생, 멈춤 등을 조작
// State 분기 조건 결정
// State 에 따른 애니메이션 결정.

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
		/// @brief 들어온 이름에 맞는 클립을 가지고 있을 경우 애니메이션을 실행 시켜 준다.
		/// @param name 클립 이름
		void Play(const tstring& name);

		/// @brief 애니메이션을 멈추고 모든 타겟오브젝트를 초기 상태로 되돌려 준다.
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
		/// @brief 애니메이션을 적용하기위한 게임오브젝트의 리스트
		/// 애니메이션이 적용되어야할 게임 오브젝트가 삭제되는 경우가 없음이 보장되어야 한다.
		/// 만약 보장되지 못한다면 매 프레임 애니메이션이 적용되어야할 게임오브젝트를 찾아야 한다.
		struct TargetList
		{
			bool bInit = false;
			std::vector<GameObject*> targets;
			std::vector<Matrix> initialLocalTMs;
		};

		/**
		 * @brief 현재 재생중인 클립의 재생 위치입니다.
		 * 이 값은 항상 클립의 길이보다 짧습니다.
		*/
		float m_PlayingTime;

		float m_PrePlayingTime; // 블랜딩이 될 이전 애니메이션의 Time

		/// @brief 애니메이션이 실행여부
		bool m_IsPlaying;

		/// @brief 현재 적용 중인 클립 캐싱
		Resource<AnimationClip> m_CurrAnimationClip;
		TargetList m_CurrTarget;
		uint32 m_CurrIndex;

		Resource<AnimationClip> m_PreAnimationClip;
		uint32 m_PreIndex;

		std::vector<Resource<AnimationClip>> m_AnimationClipList;

		// animation Transition 변수들

		bool m_IsTrasition;

		float m_Duration;

		float m_BlendStartTime;
	};
}

