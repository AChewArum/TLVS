#include "TriggerSwitch.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Collider.h"
#include "Door.h"
#include "Log/Log.h"

void TriggerSwitch::OnStart()
{
	ComponentBase::OnStart();
	for (auto _door : m_DoorList)
	{
		_door->SetGoalCounter(1);
	}
}

void TriggerSwitch::Tick()
{
	ComponentBase::Tick();
}

void TriggerSwitch::SetDoor(TL_GameEngine::GameObject* _target)
{
	if (_target == nullptr) return;
	m_DoorList.push_back(_target->GetComponent<Door>());
}

void TriggerSwitch::SetTarget(TL_GameEngine::GameObject* _target)
{
	m_Target = _target;
	m_TargetObjectName = _target->GetName();
	m_IsTarget = true;
}

void TriggerSwitch::SwitchOn()
{
	for (int i = 0; i < m_DoorList.size(); i++)
	{
		/*m_DoorList[i]->OpenDoor();*/
		m_DoorList[i]->AddCounter(1);
		m_DoorList[i]->CheckCounter();
	}
}

void TriggerSwitch::SwitchOff()
{
	for (int i = 0; i < m_DoorList.size(); i++)
	{
		m_DoorList[i]->AddCounter(-1);
		m_DoorList[i]->CheckCounter();
	}
}

void TriggerSwitch::OnTriggerEnter(const TL_GameEngine::Collider& _other)
{
	ITriggerComponent::OnTriggerEnter(_other);


	if (m_IsTarget && (m_Target == _other.GetGameObject()))
	{
		SwitchOn();
	}
	else if (m_IsTarget == false)
	{
		SwitchOn();
	}


	Log::Client_Debug("Enter");
}

void TriggerSwitch::OnTriggerStay(const TL_GameEngine::Collider& _other)
{
	ITriggerComponent::OnTriggerStay(_other);
	if (m_Is2D) { }
}

void TriggerSwitch::OnTriggerExit(const TL_GameEngine::Collider& _other)
{
	ITriggerComponent::OnTriggerExit(_other);
	if (m_IsTarget && m_Target == _other.GetGameObject())
	{
		SwitchOff();
	}
	else
	{
		SwitchOff();
	}

	Log::Client_Debug("Exit");
}
