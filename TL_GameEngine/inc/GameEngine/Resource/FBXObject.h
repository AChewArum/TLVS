#pragma once
#include "Resource.h"

namespace TL_GameEngine
{
    class GameObject;

    class FBXObject
    {
    public:
        FBXObject(
            TL_GameEngine::GameObject* _prefab,
            const std::map<tstring, Resource<StaticMesh>>& _staticMeshMap,
            const std::map<tstring, Resource<SkeletalMesh>>& _skeletalMeshMap,
            const std::vector<Resource<Material>>& _materialList)
            : m_Prefab(_prefab),
              m_StaticMeshMap(_staticMeshMap),
              m_SkeletalMeshMap(_skeletalMeshMap),
              m_MaterialList(_materialList) {}

    private:
        TL_GameEngine::GameObject* m_Prefab;

        std::map<tstring, Resource<StaticMesh>> m_StaticMeshMap;
        std::map<tstring, Resource<SkeletalMesh>> m_SkeletalMeshMap;
        std::vector<Resource<Material>> m_MaterialList;

    public:
        TL_GameEngine::GameObject* GetPrefab() const { return m_Prefab; }
        const std::map<tstring, Resource<StaticMesh>>& GetStaticMeshMap() const { return m_StaticMeshMap; }
        const std::map<tstring, Resource<SkeletalMesh>>& GetSkeletalMeshMap() const { return m_SkeletalMeshMap; }
        const std::vector<Resource<Material>>& GetMaterialList() const { return m_MaterialList; }
    };
}
