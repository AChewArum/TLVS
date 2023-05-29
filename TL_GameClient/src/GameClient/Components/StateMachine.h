#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

enum class eFSMSTATE
{
	IDLE = 0,
	WALK,
	JUMP,
	FALL,
	END
};

/**
 * \brief ���� ��ȭ�� �����ϴ� Ŭ����
 */
class StateMachine :
	public TL_GameEngine::ComponentBase
{
public:
	StateMachine(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("StateMachine"))
	:ComponentBase(_gameObject, _typeName) {};

	void OnStart() override;
	void Tick() override;

	void CurState(eFSMSTATE _State);
	void PreState(eFSMSTATE _State);

	void SetCurState(eFSMSTATE _inState); 

	eFSMSTATE GetPreState() const { return  m_PreState; }
	eFSMSTATE GetCurState() const { return  m_CurState; }

private:
	eFSMSTATE m_PreState = eFSMSTATE::IDLE;
	eFSMSTATE m_CurState = eFSMSTATE::IDLE;
};

