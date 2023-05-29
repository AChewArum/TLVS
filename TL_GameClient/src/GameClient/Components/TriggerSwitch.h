#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/Gameframework/Component/IPhysicsEventComponent.h"

namespace TL_GameEngine
{
	class Collider;
}

class Door;

class TriggerSwitch :
	public TL_GameEngine::ComponentBase, public TL_GameEngine::ITriggerComponent
{
public:
	TriggerSwitch(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("TriggerSwitch"))
		: ComponentBase(_gameObject, _typeName),
		  ITriggerComponent(this) {}

	void OnStart() override;
	void Tick() override;

	void SetDoor(TL_GameEngine::GameObject* _target);
	void SetTarget(TL_GameEngine::GameObject* _target);
	

	void SwitchOn();
	void SwitchOff();

	void OnTriggerEnter(const TL_GameEngine::Collider& _other) override;
	void OnTriggerStay(const TL_GameEngine::Collider& _other) override;
	void OnTriggerExit(const TL_GameEngine::Collider& _other) override;

	bool Is2D() const { return m_Is2D; }
	void Is2D(bool _is2D) { m_Is2D = _is2D; }

private:
	std::vector<Door*> m_DoorList;
	bool m_IsTarget = false;
	tstring m_TargetObjectName;
	TL_GameEngine::GameObject* m_Target= nullptr;
	bool m_Is2D = false;
};
