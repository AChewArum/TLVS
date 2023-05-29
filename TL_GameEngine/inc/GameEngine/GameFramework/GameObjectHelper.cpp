#include "GameEngine_pch.h"
#include "GameObjectHelper.h"

#include "GameObject.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/GameFramework/Component/SkeletalMeshRenderer.h"
#include "GameEngine/GameFramework/Component/MeshCollider.h"

namespace TL_GameEngine
{
	void GameObjectHelper::AddStaticMeshColliderRecursive(GameObject* _go)
	{
		for (auto _child : _go->GetChilds())
		{

			//_child가 _go로 되어있어서 바꿈
			auto _staticMeshRenderer = _child->GetComponent<StaticMeshRenderer>();
			if (_staticMeshRenderer != nullptr)
			{
				_child->AddComponent<MeshCollider>()->SetMesh(_staticMeshRenderer->GetMesh());
			}
			AddStaticMeshColliderRecursive(_child);
		}
	}

	void GameObjectHelper::DisableStaticMeshRendererRecursive(GameObject* _go)
	{
		for (auto _child : _go->GetChilds())
		{

			//_child가 _go로 되어있어서 바꿈
			auto _staticMeshRenderer = _child->GetComponent<StaticMeshRenderer>();
			if (_staticMeshRenderer != nullptr)
			{
				_staticMeshRenderer->SetEnable(false);
			}
			DisableStaticMeshRendererRecursive(_child);
		}
	}
}
