#include "Movement.h"

#include "GameEngine/GameFramework/GameObject.h"

void Movement::Translate(TL_GameEngine::GameObject* _target, TL_Math::Vector3 _dir)
{
	TL_Math::Vector3 _vec = _target->GetComponent<TL_GameEngine::Transform>()->GetWorldPosition();
	_vec = _vec + _dir * m_Speed;
	_target->GetComponent<TL_GameEngine::Transform>()->SetWorldPosition(_vec);
}
