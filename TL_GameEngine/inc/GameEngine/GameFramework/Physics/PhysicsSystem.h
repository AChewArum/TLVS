#pragma once

#include "Common.h"

#include "GameEngine/internal/dll.h"

#include "Math\TL_Math.h"

#include "Physics\PhysicsInfo.h"

namespace TL_Physics
{
	class IPhysicsImpl;
	class IPhysicsScene;
	class PhysicsObject;
	class PhysicsMaterial;
}

namespace TL_GameEngine
{
	class PhysicsErrorCallback;
	class TriggerComponentBase;
	class CollisionComponentBase;
	struct RaycastHit;

	//extern uint32 DefaultRaycastLayers;

	class GAME_ENGINE_API PhysicsSystem
	{
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		static PhysicsSystem& Instance();
		static void Release();

		void Initalize(TL_Physics::PhysicsInfo& physicsSetting);

		/// @brief Simulates physical scene by a elapsed time value.
		/// @param elapsedTime Amount of time to advance simulation 
		void Simulate(float elapsedTime);
		
		/// @brief Simulates physical scene by a fixed time value.
		/// @param deltaTime Amount of time to advance simulation 
		void Tick(float deltaTime);

		TL_Physics::PhysicsObject* CreateColliderObject(TL_Physics::UserData userData, TL_Physics::TransformInfo& pose, TL_Physics::ColliderInfo& colliderInfo, TL_Physics::PhysicsMaterial* physicsMaterial = nullptr);
		TL_Physics::PhysicsObject* CreateRigidBodyObject(TL_Physics::UserData userData, TL_Physics::TransformInfo& pose, TL_Physics::ColliderInfo& colliderInfo, TL_Physics::RigidInfo& rigidInfo, TL_Physics::PhysicsMaterial* physicsMaterial = nullptr);

		bool DeletePhysicsObject(TL_Physics::PhysicsObject* physicsObject);

		void AddPhysicsMessage(class Collider* collider);

		// Todo : 추후 논의가 필요함
		void AddTrigger(TriggerComponentBase* trigger);

		void AddCollisionEvent(CollisionComponentBase* collision);

		void DeleteTriggerEvent(TriggerComponentBase* trigger);

		void DeleteCollisionEvent(CollisionComponentBase* collision);

		TL_Physics::PhysicsMaterial* CreatePhysicsMaterial(TL_Physics::PhysicsMaterialInfo& materialInfo);

		void SetLayer(int i, Layer physicsLayer);

		bool Raycast(TL_Math::Vector3& orgin, TL_Math::Vector3& direction, float maxDistance = FLT_MAX, uint32 layerMask = UINT32_MAX);
		bool Raycast(TL_Math::Vector3& orgin, TL_Math::Vector3& direction, RaycastHit& hitinfo, float maxDistance = FLT_MAX, uint32 layerMask = UINT32_MAX);

		bool GetRenderBufferPoints(std::vector<TL_Physics::DebugPoint>& points);
		bool GetRenderBufferLines(std::vector<TL_Physics::DebugLine>& lines);
		bool GetRenderBufferTriangles(std::vector<TL_Physics::DebugTriangle>& triangles);

	private:
		void InitPhysicsLayer();

		bool LoadPhysicsDLL();

		void FixedUpdateMessage();

		void OnTriggerEventMessage();

		void OnCollisionEventMessage();

		void WaitForFiexedUpdateMessage();

		void FetchSimulateResult();

		void MakeCollision(class Collision& collision, const TL_Physics::CollisionEvent& event, TL_Physics::UserData* thisData, TL_Physics::UserData* otherData);

		TL_Physics::IPhysicsImpl* m_pPhysicsImpl;

		TL_Physics::PhysicsInfo m_PhysicsInfo;

		float m_fFixedTimer;

		std::vector<Collider*> m_CurrFrameColliders;

		uint32 m_MaxFramePerCollider;

		uint32 m_CurrFramePerCollider;

		std::map<uuid, TriggerComponentBase*> m_TriggerComponentBases;
		std::map<uuid, CollisionComponentBase*> m_CollisionComponentBases;

		std::shared_ptr<PhysicsErrorCallback> m_PhysicsErrorCallback;
	};
}


#define Physics TL_GameEngine::PhysicsSystem::Instance()