#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/Gameframework/Component/IPhysicsEventComponent.h"

class ObjectAttribute :
	public TL_GameEngine::ComponentBase, public TL_GameEngine::ICollisionComponent
{
public:
	ObjectAttribute(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("ObjectAttribute"))
		: ComponentBase(_gameObject, _typeName), ICollisionComponent(this) {}

	void Tick() override;
	void OnStart() override;
	void OnDestroy() override;

	bool Is2D() const { return m_Is2D; }
	void Is2D(bool _is2D) { m_Is2D = _is2D; }
	bool IsGrabbed() const { return m_IsGrabbed; }
	void IsGrabbed(bool _isGrabbed) { m_IsGrabbed = _isGrabbed; }
	bool IsCollisionEnabled() const { return m_IsCollisionEnabled; }
	void IsCollisionEnabled(bool _isCollisionEnabled) { m_IsCollisionEnabled = _isCollisionEnabled; }
	TL_Math::Vector3 CreatedLoc() const { return m_CreatedLoc; }
	void CreatedLoc(const TL_Math::Vector3& _createdLoc) { m_CreatedLoc = _createdLoc; }
	TL_Math::Vector3 CreatedScl() const { return m_CreatedScl; }
	void CreatedScl(const TL_Math::Vector3& _createdScl) { m_CreatedScl = _createdScl; }
	TL_Math::Quaternion CreatedRot() const { return m_CreatedRot; }
	void CreatedRot(const TL_Math::Quaternion& _createdRot) { m_CreatedRot = _createdRot; }
	bool Created2D() const { return m_Created2D; }
	void Created2D(bool _created2D) { m_Created2D = _created2D; }
	bool Grabable() const { return m_Grabable; }
	void Grabable(bool _grabable) { m_Grabable = _grabable; }
	bool Rotatble() const { return m_Rotatble; }
	void Rotatble(bool _rotatble) { m_Rotatble = _rotatble; }
	bool IsGravity() const { return m_IsGravity; }
	void IsGravity(bool _isGravity) { m_IsGravity = _isGravity; }
	float LinearDamping() const { return m_LinearDamping; }
	void LinearDamping(float _linearDamping) { m_LinearDamping = _linearDamping; }
	float AngularDamping() const { return m_AngularDamping; }
	void AngularDamping(float _angularDamping) { m_AngularDamping = _angularDamping; }
	float Mass() const { return m_Mass; }
	void Mass(float _mass) { m_Mass = _mass; }

	TL_GameEngine::GameObject* Model() const { return m_Model; }
	void Model(TL_GameEngine::GameObject* _model) { m_Model = _model; }

	void OnCollisionEnter(TL_GameEngine::Collision& _collision) override;
	void OnCollisionStay(TL_GameEngine::Collision& _collision) override;
	void OnCollisionExit(TL_GameEngine::Collision& _collision) override;

	bool IsCollision() const { return m_IsCollision; }


private:
	bool m_Is2D = false;
	bool m_IsGrabbed = false;
	bool m_IsCollisionEnabled = true;

	TL_Math::Vector3 m_CreatedLoc;
	TL_Math::Vector3 m_CreatedScl;
	TL_Math::Quaternion m_CreatedRot;
	bool m_Created2D = false;
	bool m_Grabable = true;
	bool m_Rotatble = true;

	bool m_IsGravity = true;
	float m_LinearDamping;
	float m_AngularDamping;
	float m_Mass;

	TL_Math::Vector3 m_InvDir;

	bool m_IsCollision = false;
	TL_GameEngine::GameObject* m_Model = nullptr;
};
