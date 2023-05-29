#include "AnimationController.h"

#include "StateMachine.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/Gameframework/Component/Animator.h"
#include "GameEngine/GameFramework/Component/RigidBody.h"

namespace Physics {
	class RigidBody;
}

void AnimationController::OnStart()
{
	ComponentBase::OnStart();

	m_PlayerState = GetGameObject()->GetComponent<StateMachine>();
	m_PlayerAnimator = GetGameObject()->GetChild(TEXT("PlayerModel"))->GetChild(TEXT("Armature"))->GetComponent<TL_GameEngine::Animator>();

	if(m_PlayerAnimator->GetAnimationClipList().size() == 0)
	{
		return;
	}
	else if (m_PlayerAnimator->GetAnimationClipList().size() == 1)
	{
		SetStateAnimationList(eFSMSTATE::IDLE, 0);

		const auto _animName = m_StateAnimNameList.at(eFSMSTATE::IDLE);
		m_PlayerAnimator->Play(_animName);				// 애니메이션 초기값 설정
	}
	else
	{
		SetStateAnimationList(eFSMSTATE::IDLE, 0);
		SetStateAnimationList(eFSMSTATE::WALK, 1);
		SetStateAnimationList(eFSMSTATE::JUMP, 2);
	
		SetFloat(TEXT("Speed"), 0.0f);		// 속도 초기값 설정
	
		const auto _animName = m_StateAnimNameList.at(eFSMSTATE::IDLE);
		m_PlayerAnimator->Play(_animName);				// 애니메이션 초기값 설정
	
		SetStateCondition(eFSMSTATE::IDLE, eFSMSTATE::WALK,
			[](AnimationController* _anim)
			{
				// m_FloatMap.at(TEXT("speed"))이 1보다 크면 walk 전이
				if (_anim->m_FloatMap.at(TEXT("Speed")) > 1.f) return true;
				return false;
			});
		SetStateCondition(eFSMSTATE::WALK, eFSMSTATE::IDLE,
			[](AnimationController* _anim)
			{
				// m_FloatMap.at(TEXT("speed"))이 1보다 작으면 idle 전이
				if (1.f > _anim->m_FloatMap.at(TEXT("Speed"))) return true;
				return false;
			});
	}
}

void AnimationController::Tick()
{
	ComponentBase::Tick();

	/////// AnimationController를 변수들을 Set해주는 부분 구현중 ///////
	const auto _posX = m_GameObject->GetComponent<TL_GameEngine::RigidBody>()->GetLinearVelocity().x;
	const auto _posZ = m_GameObject->GetComponent<TL_GameEngine::RigidBody>()->GetLinearVelocity().z;
	const float _speed = abs(sqrt(pow(_posX, 2) + pow(_posZ, 2)));
	if (m_GameObject->GetComponent<AnimationController>())
	{
		m_GameObject->GetComponent<AnimationController>()->SetFloat(TEXT("Speed"), _speed);
	}

	// 현재 State를 계속 check하면서 조건을 검사하자. 
	CheckCondition(m_PlayerState->GetCurState());
}

void AnimationController::SetStateAnimationList(eFSMSTATE _state, int _animIndex)
{
	auto _animName = m_PlayerAnimator->GetAnimationClip(_animIndex)->GetClipName();

	m_StateAnimNameList.insert({ _state, _animName });
}

void AnimationController::SetStateCondition(const eFSMSTATE _startState, const eFSMSTATE _destinationState, std::function<bool(AnimationController*)> _func)
{
	TransitionInfo _newTransition;

	_newTransition.m_StartState = _startState;
	_newTransition.m_DestinationState = _destinationState;
	_newTransition.SetCondition(_func);

	m_TransitionInfosList.push_back(_newTransition);
}

void AnimationController::CheckCondition(const eFSMSTATE _startState)
{
	if (m_TransitionInfosList.size() == 0) return;

	for (auto _transition : m_TransitionInfosList)
	{
		// 현재 State가 같을 때 condition 검사. 
		if (_transition.m_StartState == _startState)
		{
			// condition 실행
			const auto _check = _transition.m_Condition(this);

			if (_check == true)
			{
				auto _animStartName = m_StateAnimNameList.at(_transition.m_StartState);
				auto _animTargetName = m_StateAnimNameList.at(_transition.m_DestinationState);

				// 1. 상태 및 애니메이션 변환
				//	1-1. state 전이.
				m_PlayerState->SetCurState(_transition.m_DestinationState);

				//	1-3. Animator Play clip 변경, 이전 애니메이션과 현재 재생될 애니메이션 블렌딩
				m_PlayerAnimator->StartTransition(_animTargetName);
			}
			else
			{
				// 2.현 상태 유지
				auto _animName = m_StateAnimNameList.at(_transition.m_StartState);
				m_PlayerAnimator->Play(_animName);
			}
		}
	}
}

void AnimationController::SetFloat(const tstring& _name, const float _float)
{
	const auto _iterator = m_FloatMap.find(_name);

	if ( _iterator != m_FloatMap.end())
	{
		m_FloatMap.erase(_iterator);
	}

	m_FloatMap.insert({ _name, _float });
}

void AnimationController::SetInt(const tstring& _name, const int _int)
{
	const auto _iterator = m_IntMap.find(_name);

	if (_iterator != m_IntMap.end())
	{
		m_IntMap.erase(_iterator);
	}

	m_IntMap.insert({ _name, _int });
}

void AnimationController::SetBool(const tstring& _name, const bool _bool)
{
	const auto _iterator = m_BoolMap.find(_name);

	if (_iterator != m_BoolMap.end())
	{
		m_BoolMap.erase(_iterator);
	}

	m_BoolMap.insert({ _name, _bool });
}

void AnimationController::SetTrigger(const tstring& _name, const bool _trigger)
{
	const auto _iterator = m_TriggerMap.find(_name);

	if (_iterator != m_TriggerMap.end())
	{
		m_TriggerMap.erase(_iterator);
	}

	m_TriggerMap.insert({ _name, _trigger });
}
