#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

class Door :
	public TL_GameEngine::ComponentBase
{
public:
	Door(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("Door"))
		: ComponentBase(_gameObject, _typeName) {}

	void OnStart() override;
	void Tick() override;
	void FixedTick() override;

	void OpenDoor();
	void CloseDoor();

	void SetGoalCounter(int _amount) { m_GoalCounter += _amount; }

	void CheckCounter();
	void AddCounter(int _amount);


private:
	bool m_IsOpen = false;
	int m_Counter = 0;
	int m_GoalCounter = 0;
	float m_Y = 0;
	bool m_AnimOn = false;
};
