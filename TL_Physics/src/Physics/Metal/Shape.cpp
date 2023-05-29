#include "..\PhysicsPCH.h"
#include "Shape.h"
#include "Physics\PhysicsImpl.h"
#include "Physics\Metal\PhysicsMaterial.h"


TL_Physics::Shape::Shape()
{

}

TL_Physics::Shape::~Shape()
{
	SAFE_RELEASE(m_pShape)
}

bool TL_Physics::Shape::CreateBoxShape(BoxColliderInfo& boxColliderInfo, physx::PxMaterial* pxMaterial)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (boxColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxBoxGeometry(boxColliderInfo._size.x * 0.5f, boxColliderInfo._size.y * 0.5f, boxColliderInfo._size.z * 0.5f),
		*pxMaterial, true, flags);

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들
	m_pShape->setSimulationFilterData(filterData);

	m_ColliderInfo = &boxColliderInfo;
	return true;
}

bool TL_Physics::Shape::CreateSphereShape(SphereColliderInfo& sphereColliderInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (sphereColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxSphereGeometry(sphereColliderInfo._radius),
		*pxMaterial, true, flags);

	m_ColliderInfo = &sphereColliderInfo;

	return true;
}

bool TL_Physics::Shape::CreateCapsuleShape(CapsuleColliderInfo& capsuleColliderInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (capsuleColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxCapsuleGeometry(capsuleColliderInfo._radius, capsuleColliderInfo._height),
		*pxMaterial, true, flags);

	m_ColliderInfo = &capsuleColliderInfo;

	return true;
}

bool TL_Physics::Shape::CreateMeshShape(MeshColliderInfo& meshColliderInfo, TransformInfo& transformInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	physx::PxMeshScale _meshScale = { Vector3ToPxVec3(transformInfo._scale) };

	if (meshColliderInfo._convex)
	{
		if (meshColliderInfo._isTrigger)
		{
			flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
		}

		physx::PxConvexMeshDesc _convexDesc;

		if (meshColliderInfo._triCount == 0 ||
			meshColliderInfo._triStride == 0 ||
			meshColliderInfo._triData == nullptr)
		{
			_convexDesc.points.count = meshColliderInfo._count;
			_convexDesc.points.stride = meshColliderInfo._stride;
			_convexDesc.points.data = meshColliderInfo._meshData;

			_convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;// | physx::PxConvexFlag::eDISABLE_MESH_VALIDATION;
			_convexDesc.vertexLimit = 255;
		}
		else
		{
			_convexDesc.polygons.count = meshColliderInfo._count;
			_convexDesc.polygons.stride = meshColliderInfo._stride;
			_convexDesc.polygons.data = meshColliderInfo._meshData;

			_convexDesc.indices.count = meshColliderInfo._triCount;
			_convexDesc.indices.stride = meshColliderInfo._triStride;
			_convexDesc.indices.data = meshColliderInfo._triData;
		}

		physx::PxDefaultMemoryOutputStream _buf;

		physx::PxConvexMeshCookingResult::Enum _result;

		if (!PhysicsSDK->GetCooking()->cookConvexMesh(_convexDesc, _buf, &_result))
		{
			AssertMessageBox(false, "Convex mesh cooking faild");
			return false;
		}

		if (_result == physx::PxConvexMeshCookingResult::eFAILURE
			|| _result == physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED)
		{
			AssertMessageBox(false, "Convex mesh cooking faild");
			return false;
		}


		physx::PxDefaultMemoryInputData _input(_buf.getData(), _buf.getSize());
		physx::PxConvexMesh* _convexMesh = PhysicsSDK->GetPhysics()->createConvexMesh(_input);

		m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxConvexMeshGeometry(_convexMesh, _meshScale),
			*pxMaterial, true, flags);
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;

		physx::PxTriangleMeshDesc _triangleDesc;
		
		_triangleDesc.points.count = meshColliderInfo._count;
		_triangleDesc.points.stride = meshColliderInfo._stride;
		_triangleDesc.points.data = meshColliderInfo._meshData;

		_triangleDesc.triangles.count = meshColliderInfo._triCount;
		_triangleDesc.triangles.stride = meshColliderInfo._triStride;
		_triangleDesc.triangles.data = meshColliderInfo._triData;

		_triangleDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

		physx::PxDefaultMemoryOutputStream _buf;
		physx::PxTriangleMeshCookingResult::Enum _result;

		if (!PhysicsSDK->GetCooking()->cookTriangleMesh(_triangleDesc, _buf, &_result))
		{
			AssertMessageBox(false, "Triangle mesh cooking faild");
			return false;
		}

		physx::PxDefaultMemoryInputData _readBuffer(_buf.getData(), _buf.getSize());
		
		physx::PxTriangleMesh* _triMesh = PhysicsSDK->GetPhysics()->createTriangleMesh(_readBuffer);

		m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxTriangleMeshGeometry(_triMesh, _meshScale),
			*pxMaterial, true, flags);
	}

	m_ColliderInfo = &meshColliderInfo;

	return true;
}

bool TL_Physics::Shape::CreateHeightFieldShape(HeightFieldColliderInfo& heightFieldInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	using namespace physx;

	assert(false);

	/*PxHeightFieldDesc _hfDesc;
	_hfDesc.format = PxHeightFieldFormat::eS16_TM;
	_hfDesc.nbColumns = heightFieldInfo._column;
	_hfDesc.nbRows = heightFieldInfo._row;
	_hfDesc.samples.data = samples;
	_hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField* aHeightField = theCooking->createHeightField(_hfDesc,
		thePhysics->getPhysicsInsertionCallback());*/

	return true;
}

bool TL_Physics::Shape::SetLayer(physx::PxFilterData& filterData)
{
	NullCheck(m_pShape);

	m_pShape->setSimulationFilterData(filterData);

	physx::PxFilterData _queryFilter;
	_queryFilter.word0 = filterData.word0;

	m_pShape->setQueryFilterData(filterData);
	return true;
}

bool TL_Physics::Shape::SetIsTrigger(bool flag)
{
	NullCheck(m_pShape);

	if (CheckType(eShapeType::MESH) && ! reinterpret_cast<MeshColliderInfo*>(m_ColliderInfo)->_convex)
	{
		return false;
	}

	if(CheckType(eShapeType::HEIGHTFIELD))
	{
		return false;
	}

	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !flag);
	m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, flag);

	return true;
}

void TL_Physics::Shape::SetCenter(TL_Math::Vector3 center)
{
	// 이후에 비트 연산으로 바꾸고싶다.
	if (CheckType(eShapeType::MESH))
		return;

	NullCheck(m_pShape);

	physx::PxTransform _pose;

	_pose.p = Vector3ToPxVec3(center);
	_pose.q = physx::PxQuat(physx::PxIDENTITY::PxIdentity);
	m_pShape->setLocalPose(_pose);
}

void TL_Physics::Shape::SetSize(TL_Math::Vector3 size)
{
	// 이후에 비트 연산으로 바꾸고싶다.
	if (!CheckType(eShapeType::BOX))
		return;

	NullCheck(m_pShape)

	auto geo = physx::PxBoxGeometry(physx::PxVec3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));

	m_pShape->setGeometry(geo);
}

void TL_Physics::Shape::SetRaduis(float raduis)
{
	NullCheck(m_pShape)

	if (CheckType(eShapeType::CAPSULE))
	{
		physx::PxCapsuleGeometry geo;
		m_pShape->getCapsuleGeometry(geo);

		geo.radius = raduis;
		m_pShape->setGeometry(geo);
	}
	else if (CheckType(eShapeType::SPHERE))
	{
		physx::PxSphereGeometry geo;
		m_pShape->getSphereGeometry(geo);

		geo.radius = raduis;
		m_pShape->setGeometry(geo);
	}
}

void TL_Physics::Shape::SetHeight(float height)
{
	if (!CheckType(eShapeType::BOX))
		return;

	NullCheck(m_pShape)

	physx::PxCapsuleGeometry geo;
	m_pShape->getCapsuleGeometry(geo);

	geo.halfHeight = height / 2;
	m_pShape->setGeometry(geo);
}

void TL_Physics::Shape::SetAxisDirection(eAxisDirection dir)
{
	if (!CheckType(eShapeType::BOX))
		return;

	NullCheck(m_pShape)

	assert(false);
}

bool TL_Physics::Shape::SetPhysicsMaterial(PhysicsMaterial* physicsMaterial)
{
	NullCheck(m_pShape);

	physx::PxMaterial* _meterials[] = { physicsMaterial->GetMaterial() };

	m_pShape->setMaterials(_meterials, 0);

	return true;
}

bool TL_Physics::Shape::CheckType(eShapeType shapeType)
{
	return (shapeType == m_ColliderInfo->_shapeType);
}
