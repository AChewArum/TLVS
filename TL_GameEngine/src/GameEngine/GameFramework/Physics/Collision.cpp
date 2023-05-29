#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Physics\Collision.h"

namespace TL_GameEngine
{
	Collision::Collision()
	: m_iContactCount(0)
	, m_pRigidBody(nullptr)
	, m_pCollider(nullptr)
	, m_pGameObject(nullptr)
	, m_Impulse{ 0.0f, 0.0f, 0.0f }
	, m_RelativeVelocity{ 0.0f, 0.0f, 0.0f }
	{

	}

	Collision::~Collision()
	{

	}

	inline ContactPoint Collision::GetContact(int index)
	{
		if (m_iContactCount < index)
		{
			return m_ContactPoints[index];
		}
		else
		{
			return ContactPoint();
		}
	}
}
