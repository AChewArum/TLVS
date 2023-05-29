#include "Door.h"

#include "GameEngine/GameFramework/GameObject.h"

using namespace TL_GameEngine;

void Door::OnStart()
{
	ComponentBase::OnStart();
	m_Y = m_GameObject->GetComponent<Transform>()->GetWorldPosition().y;
}

void Door::Tick()
{
	ComponentBase::Tick();
	
}

void Door::FixedTick()
{
	ComponentBase::FixedTick();
	if (m_AnimOn)
	{
		auto _v = m_GameObject->m_Transform.GetWorldPosition();

		if (m_IsOpen)
		{
			if (_v.y <= m_Y + 50)
			{
				_v.y = _v.y + 0.2;
				m_GameObject->m_Transform.SetWorldPosition(_v);
				return;
			}
			m_AnimOn = false;
		}
		else
		{
			if (_v.y >= m_Y)
			{
				_v.y = _v.y - 0.2;
				m_GameObject->m_Transform.SetWorldPosition(_v);
				return;
			}
			m_AnimOn = false;
		}
	}
}

void Door::OpenDoor()
{
	m_IsOpen = true;

	m_AnimOn = true;
}

void Door::CloseDoor()
{
	m_IsOpen = false;

	m_AnimOn = true;
}

void Door::CheckCounter()
{
	if (m_Counter == m_GoalCounter)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

void Door::AddCounter(int _amount)
{
	m_Counter = std::clamp(m_Counter + _amount, 0, m_GoalCounter);
}
