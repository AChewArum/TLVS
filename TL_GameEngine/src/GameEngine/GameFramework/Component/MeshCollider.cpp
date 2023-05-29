#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\MeshCollider.h"
#include "GameEngine\Resource\Mesh.h"


namespace TL_GameEngine
{
	MeshCollider::MeshCollider(GameObject* _gameObject, const tstring& _typeName)
		: Collider(_gameObject, _typeName, m_MeshColliderInfo)
	{

	}

	MeshCollider::~MeshCollider()
	{

	}

	void MeshCollider::PreTick()
	{
		__super::PreTick();
	}

	void MeshCollider::Tick()
	{
		__super::Tick();
	}

	void MeshCollider::PostTick()
	{
		__super::PostTick();
	}

	void MeshCollider::TryTick()
	{
		__super::TryTick();
	}

	void MeshCollider::OnStart()
	{
		__super::OnStart();
	}

	void MeshCollider::OnChangeEnable(bool _newEnable)
	{
		__super::OnChangeEnable(_newEnable);
	}

	void MeshCollider::SetConvex(bool flag)
	{
		m_IsDirty = true;

		m_MeshColliderInfo._convex = flag;
	}

	bool MeshCollider::GetConvex()
	{
		return m_MeshColliderInfo._convex;
	}

	void MeshCollider::SetIsTrigger(bool flag)
	{
		m_MeshColliderInfo._convex ? m_MeshColliderInfo._isTrigger = flag : m_MeshColliderInfo._isTrigger = false;
	}

	bool MeshCollider::GetIsTrigger(bool flag)
	{
		return m_MeshColliderInfo._convex ? m_MeshColliderInfo._isTrigger : false;
	}

	void MeshCollider::SetMesh(Resource<StaticMesh> meshRes)
	{
		m_IsDirty = true;

		m_pMesh = meshRes;

		m_MeshData.clear();

		for (auto& vertex : meshRes.GetPtr()->GetVertexList())
		{
			m_MeshData.push_back(vertex.position.x);
			m_MeshData.push_back(vertex.position.y);
			m_MeshData.push_back(vertex.position.z);
		}

		m_MeshColliderInfo._meshData = m_MeshData.data();
		m_MeshColliderInfo._stride = sizeof(float) * 3;
		m_MeshColliderInfo._count = static_cast<uint32>(m_pMesh.GetPtr()->GetVertexList().size());

		m_MeshColliderInfo._triData = const_cast<uint32*>(m_pMesh->GetIndexList().data());
		m_MeshColliderInfo._triStride = sizeof(float) * 3;
		m_MeshColliderInfo._triCount = m_pMesh.GetPtr()->GetIndexList().size();
	}

	Resource<StaticMesh> MeshCollider::GetMesh()
	{
		return m_pMesh;
	}
}