#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

namespace TL_GameEngine {
	class Animator;
}

class StateMachine;
enum class eFSMSTATE;

/**
 * \brief 플레이어의 상태에 따른 애니메이션 클립을 바꿔주는 클래스
 */

class AnimationController :
	public TL_GameEngine::ComponentBase
{
public:
	AnimationController(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("AnimationController"))
		:ComponentBase(_gameObject, _typeName), m_PlayerAnimator(nullptr), m_PlayerState(nullptr), m_HasExitTime(false), m_TransitionDuration(0.0f), m_TransitionOffset(0.0f) {};

	void OnStart() override;
	void Tick() override;

	void SetStateAnimationList(eFSMSTATE _state, int _animIndex); // 어떤 상태일때 어떤 애니를 틀건지 setting

	void SetStateCondition(const eFSMSTATE _startState, const eFSMSTATE _destinationState, std::function<bool(AnimationController*)> _func);

	void CheckCondition(const eFSMSTATE _startState);

	void SetFloat(const tstring& _name, const float _float);
	void SetInt(const tstring& _name, const int _int);
	void SetBool(const tstring& _name, const bool _bool);
	void SetTrigger(const tstring& _name, const bool _trigger);

public:
	TL_GameEngine::Animator* m_PlayerAnimator;
	StateMachine* m_PlayerState;

	// 상태에 따른 애니메이션과 전이 될 상태, 애니메이션의 정보, 전이 조건
	struct TransitionInfo
	{
		eFSMSTATE m_StartState;
		eFSMSTATE m_DestinationState; // 전이 될 상태.

		std::function<bool(AnimationController*)> m_Condition;

		void SetCondition(std::function<bool(AnimationController*)> _func)
		{
			m_Condition = _func;
		}
	};

	std::vector<TransitionInfo> m_TransitionInfosList;

	std::map<eFSMSTATE, tstring> m_StateAnimNameList; // 상태일 때 틀어줄 애니메이션 이름 목록.

	// State분기에 사용될 parameter
	std::map<tstring, float> m_FloatMap;
	std::map<tstring, int> m_IntMap;
	std::map<tstring, bool> m_BoolMap;
	std::map<tstring, bool> m_TriggerMap;

	// animation Transition 에 사용될 변수
	bool m_HasExitTime; // 상태가 변해도 애니메이션 루프를 끝까지 돌릴지 여부.

	float m_TransitionDuration;	// blending Time
	float m_TransitionOffset;	// blended Start Time in next anim
};

