#include "PhysicsPCH.h"
#include "Physics\PhysicsObject.h"
#include "Physics\PhysicsInfo.h"
#include "Physics\Metal\RigidActor.h"
#include "Physics\Metal\Shape.h"
#include "Physics\Metal\PhysicsMaterial.h"
#include "PhysicsImpl.h"
#include "PhysicsScene.h"

TL_Physics::PhysicsObject::PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, PhysicsMaterial* material)
	: m_pRigidActor(nullptr)
	, m_pShape(nullptr)
	, m_ColliderInfoRef(colliderInfo)
{
	m_IsVaild = CreateShape(colliderInfo, transformInfo, material);
	m_IsVaild = CreateColliderActor(userData, transformInfo);
}

TL_Physics::PhysicsObject::PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* material)
	: m_pRigidActor(nullptr)
	, m_pShape(nullptr)
	, m_ColliderInfoRef(colliderInfo)
	, m_RigidInfo(rigidInfo)
{
	m_IsVaild = CreateShape(colliderInfo, transformInfo, material);
	m_IsVaild = CreateRigidActor(userData, transformInfo, rigidInfo);
}

TL_Physics::PhysicsObject::~PhysicsObject()
{
	PhysicsSDK->Instance()->GetScene()->GetScene()->removeActor(*(m_pRigidActor->GetActor()));

	SAFE_DELETE(m_pRigidActor)
	SAFE_DELETE(m_pShape)
}

bool TL_Physics::PhysicsObject::CreateShape(ColliderInfo& colliderInfo, TransformInfo& transformInfo, PhysicsMaterial* physicsMaterial)
{
	m_pShape = new Shape();
	
	if (physicsMaterial == nullptr)
	{
		physicsMaterial = PhysicsSDK->GetDefaultMaterial();
	}

	switch (colliderInfo._shapeType)
	{
		case eShapeType::BOX:
		{
			m_pShape->CreateBoxShape(reinterpret_cast<BoxColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::CAPSULE:
		{
			m_pShape->CreateCapsuleShape(reinterpret_cast<CapsuleColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::SPHERE:
		{
			m_pShape->CreateSphereShape(reinterpret_cast<SphereColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::MESH:
		{
			m_pShape->CreateMeshShape(reinterpret_cast<MeshColliderInfo&>(colliderInfo), transformInfo, physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::HEIGHTFIELD:
		{
			m_pShape->CreateHeightFieldShape(reinterpret_cast<HeightFieldColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		default:
		{
			AssertMessageBox(false, "Shape type is unknown");
			break;
		}
	}

	return true;
}

bool TL_Physics::PhysicsObject::CreateRigidActor(UserData& userData, TransformInfo& transformInfo, RigidInfo& rigidInfo)
{
	m_pRigidActor = new RigidActor(userData, transformInfo, m_pShape->GetShape(), rigidInfo);

	NullCheck(m_pRigidActor);

	PhysicsSDK->Instance()->GetScene()->GetScene()->addActor(*(m_pRigidActor->GetActor()));

	return true;
}

bool TL_Physics::PhysicsObject::CreateColliderActor(UserData& userData, TransformInfo& transformInfo)
{
	m_pRigidActor = new RigidActor(userData, transformInfo, m_pShape->GetShape());

	NullCheck(m_pRigidActor);

	PhysicsSDK->Instance()->GetScene()->GetScene()->addActor(*(m_pRigidActor->GetActor()));

	return true;
}

bool TL_Physics::PhysicsObject::OnEnable()
{
	return m_pRigidActor->OnEnable();
}

bool TL_Physics::PhysicsObject::OnDisable()
{
	return m_pRigidActor->OnDisable();
}

void TL_Physics::PhysicsObject::SetTransform(TransformInfo& transformInfo)
{
	NullCheck(m_pRigidActor)

	m_pRigidActor->SetTransform(transformInfo);
}

TL_Physics::TransformInfo TL_Physics::PhysicsObject::GetTransform()
{
	NullCheck(m_pRigidActor)

	return m_pRigidActor->GetTransform();
}

bool TL_Physics::PhysicsObject::SetLayer(int i)
{
	NullCheck(m_pShape);

	m_pShape->SetLayer(reinterpret_cast<PxFilterData&>(PhysicsSDK->GetFilter(i)));

	return true;
}

void TL_Physics::PhysicsObject::SetIsTrigger(bool val)
{
	NullCheck(m_pShape)

	m_pShape->SetIsTrigger(val);
}

void TL_Physics::PhysicsObject::SetCenter(TL_Math::Vector3 center)
{
	NullCheck(m_pShape)

	m_pShape->SetCenter(center);
}

void TL_Physics::PhysicsObject::SetSize(TL_Math::Vector3 size)
{
	NullCheck(m_pShape)

	m_pShape->SetSize(size);
}

void TL_Physics::PhysicsObject::SetRadius(float radius)
{
	NullCheck(m_pShape)

	m_pShape->SetRaduis(radius);
}

void TL_Physics::PhysicsObject::SetHeight(float height)
{
	NullCheck(m_pShape);

	m_pShape->SetHeight(height);
}

void TL_Physics::PhysicsObject::SetAxisDirection(eAxisDirection dir)
{
	NullCheck(m_pShape);

	m_pShape->SetAxisDirection(dir);
}

bool TL_Physics::PhysicsObject::SetMass(float mass)
{
	NullCheck(m_pShape);

	return m_pRigidActor->SetMass(mass);
}

bool TL_Physics::PhysicsObject::SetLinearDamping(float damping)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetLinearDamping(damping);
}

bool TL_Physics::PhysicsObject::SetAngularDamping(float damping)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetAngularDamping(damping);
}

bool TL_Physics::PhysicsObject::SetGravity(bool flag)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetGravity(flag);
}

bool TL_Physics::PhysicsObject::SetKinematic(bool flag)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetKinematic(flag);
}

bool TL_Physics::PhysicsObject::SetConstraints(Constraints& constraints)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetConstraints(constraints);
}

bool TL_Physics::PhysicsObject::SetCollisionDetection(eCollisionDetection& collisionDetection)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetCollisionDetection(collisionDetection);
}

bool TL_Physics::PhysicsObject::SetLinearVelocity(TL_Math::Vector3 velocity)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetLinearVelocity(velocity);
}

bool TL_Physics::PhysicsObject::SetAngularVelocity(TL_Math::Vector3 velocity)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetAngularVelocity(velocity);
}

bool TL_Physics::PhysicsObject::SetMaxLinearVelocity(float val)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetMaxLinearVelocity(val);
}

bool TL_Physics::PhysicsObject::SetMaxAngularVelocity(float val)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetMaxAngularVelocity(val);
}

bool TL_Physics::PhysicsObject::AddForce(TL_Math::Vector3 vector, int forceMode)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->AddForce(vector, forceMode);
}

bool TL_Physics::PhysicsObject::AddTorque(TL_Math::Vector3 torque, int forceMode)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->AddTorque(torque, forceMode);
}

bool TL_Physics::PhysicsObject::SetPhysicsMaterial(TL_Physics::PhysicsMaterial* physicsMaterial)
{
	NullCheck(m_pShape);

	return m_pShape->SetPhysicsMaterial(physicsMaterial);
}

TL_Math::Vector3 TL_Physics::PhysicsObject::GetLinearVelocity()
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->GetLinearVelocity();
}

TL_Math::Vector3 TL_Physics::PhysicsObject::GetAngularVelocity()
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->GetAngularVelocity();
}
