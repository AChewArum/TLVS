#pragma once
#include "GameEngine/GameApplication.h"


namespace TL_GameEngine
{
	class GameObject;
	class Scene;
}

using namespace TL_GameEngine;

class Stage29 :
	public GameApplication
{
public:
	void OnApplicationStart() override;
	void OnApplicationTick() override;
	void OnApplicationEnd() override;

private:
	Scene* m_Scene = nullptr;
	GameObject* m_PlayerObj = nullptr;
	GameObject* m_TargetObj = nullptr;

	GameObject* SpawnPlayer();
	GameObject* SpawnCamera(const tstring& _name) const;
	GameObject* SpawnStageObject() const;
	GameObject* SpawnInteractableObject(TL_Math::Vector3 _loc, const tstring& _path);

	static void SetPhysicsLayers();

	GameObject* SpawnDoor(TL_Math::Vector3 _loc);
	GameObject* SpawnSwitch(TL_Math::Vector3 _loc,const tstring& _path);
};

