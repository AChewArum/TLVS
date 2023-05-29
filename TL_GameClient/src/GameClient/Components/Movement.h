#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

class Movement :
	public TL_GameEngine::ComponentBase
{
public:
	Movement(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("Movement"))
		: ComponentBase(_gameObject, _typeName) {}

	void Translate(TL_GameEngine::GameObject* _target, TL_Math::Vector3 _dir);
	float Speed() const { return m_Speed; }
	void Speed(float _speed) { m_Speed = _speed; }


protected:
	float m_Speed = 0.05f;
};
