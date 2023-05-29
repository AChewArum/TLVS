#include "ObjectAttribute.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/RigidBody.h"
#include "Log/Log.h"

using namespace TL_GameEngine;

void ObjectAttribute::Tick()
{
	ComponentBase::Tick();
}

void ObjectAttribute::OnStart()
{
	ComponentBase::OnStart();
	m_CreatedScl = m_Model->m_Transform.GetWorldScale();
	m_CreatedLoc = m_Model->m_Transform.GetWorldPosition();
	m_CreatedRot = m_Model->m_Transform.GetWorldRotation();


}

void ObjectAttribute::OnDestroy()
{
	ComponentBase::OnDestroy();

}

void ObjectAttribute::OnCollisionEnter(TL_GameEngine::Collision& _collision)
{
	ICollisionComponent::OnCollisionEnter(_collision);


	
	Log::Client_Debug("ENTER");
}

void ObjectAttribute::OnCollisionStay(TL_GameEngine::Collision& _collision)
{
	ICollisionComponent::OnCollisionStay(_collision);
	
}

void ObjectAttribute::OnCollisionExit(TL_GameEngine::Collision& _collision)
{
	ICollisionComponent::OnCollisionExit(_collision);

	Log::Client_Debug("EXIT");
	m_IsCollision = false;
}
