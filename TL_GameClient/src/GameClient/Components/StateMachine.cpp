#include "StateMachine.h"

#include "CharacterMovement.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "Log/Log.h"

void StateMachine::OnStart()
{
	ComponentBase::OnStart();
	m_CurState = eFSMSTATE::IDLE;
}

void StateMachine::Tick()
{
	ComponentBase::Tick();
}


void StateMachine::CurState(eFSMSTATE _State)
{
	switch (_State)
	{
	case eFSMSTATE::IDLE:
		{
			Log::Engine_Info("Idle ¡¯¿‘");
		}
		break;

	case eFSMSTATE::WALK:
		{
			Log::Engine_Info("Walk ¡¯¿‘");
		}
		break;

	case eFSMSTATE::JUMP:
		{
			Log::Engine_Info("jump ¡¯¿‘");
		}
		break;

	case eFSMSTATE::END:
		{
			Log::Engine_Info("End ¡¯¿‘");
		}
		break;
	}
}

void StateMachine::PreState(eFSMSTATE _State)
{
	switch (_State)
	{
	case eFSMSTATE::IDLE:
		{
			Log::Engine_Info("Idle ≈ª√‚");
		}
		break;

	case eFSMSTATE::WALK:
		{
			Log::Engine_Info("Walk ≈ª√‚");
		}
		break;

	case eFSMSTATE::JUMP:
		{
			Log::Engine_Info("jump ≈ª√‚");
		}
		break;

	case eFSMSTATE::END:
		{
			Log::Engine_Info("End ≈ª√‚");
		}
		break;
	}
}

void StateMachine::SetCurState(eFSMSTATE _inState)
{
	if(m_CurState != _inState)
	{
		m_PreState = m_CurState;
		m_CurState = _inState;
		PreState(m_PreState);
		CurState(m_CurState);
	}
}
