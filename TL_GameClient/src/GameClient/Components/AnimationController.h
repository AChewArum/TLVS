#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

namespace TL_GameEngine {
	class Animator;
}

class StateMachine;
enum class eFSMSTATE;

/**
 * \brief �÷��̾��� ���¿� ���� �ִϸ��̼� Ŭ���� �ٲ��ִ� Ŭ����
 */

class AnimationController :
	public TL_GameEngine::ComponentBase
{
public:
	AnimationController(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("AnimationController"))
		:ComponentBase(_gameObject, _typeName), m_PlayerAnimator(nullptr), m_PlayerState(nullptr), m_HasExitTime(false), m_TransitionDuration(0.0f), m_TransitionOffset(0.0f) {};

	void OnStart() override;
	void Tick() override;

	void SetStateAnimationList(eFSMSTATE _state, int _animIndex); // � �����϶� � �ִϸ� Ʋ���� setting

	void SetStateCondition(const eFSMSTATE _startState, const eFSMSTATE _destinationState, std::function<bool(AnimationController*)> _func);

	void CheckCondition(const eFSMSTATE _startState);

	void SetFloat(const tstring& _name, const float _float);
	void SetInt(const tstring& _name, const int _int);
	void SetBool(const tstring& _name, const bool _bool);
	void SetTrigger(const tstring& _name, const bool _trigger);

public:
	TL_GameEngine::Animator* m_PlayerAnimator;
	StateMachine* m_PlayerState;

	// ���¿� ���� �ִϸ��̼ǰ� ���� �� ����, �ִϸ��̼��� ����, ���� ����
	struct TransitionInfo
	{
		eFSMSTATE m_StartState;
		eFSMSTATE m_DestinationState; // ���� �� ����.

		std::function<bool(AnimationController*)> m_Condition;

		void SetCondition(std::function<bool(AnimationController*)> _func)
		{
			m_Condition = _func;
		}
	};

	std::vector<TransitionInfo> m_TransitionInfosList;

	std::map<eFSMSTATE, tstring> m_StateAnimNameList; // ������ �� Ʋ���� �ִϸ��̼� �̸� ���.

	// State�б⿡ ���� parameter
	std::map<tstring, float> m_FloatMap;
	std::map<tstring, int> m_IntMap;
	std::map<tstring, bool> m_BoolMap;
	std::map<tstring, bool> m_TriggerMap;

	// animation Transition �� ���� ����
	bool m_HasExitTime; // ���°� ���ص� �ִϸ��̼� ������ ������ ������ ����.

	float m_TransitionDuration;	// blending Time
	float m_TransitionOffset;	// blended Start Time in next anim
};

