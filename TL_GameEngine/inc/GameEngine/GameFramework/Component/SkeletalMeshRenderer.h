#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/Resource/Resource.h"
#include "Math/TL_Math.h"

namespace TL_GameEngine
{
    struct Bone
    {
        GameObject* go;
        Matrix offsetTM;
    };

    class GAME_ENGINE_API SkeletalMeshRenderer :
        public ComponentBase,
        public IRenderableComponent
    {
    public:
        SkeletalMeshRenderer(class GameObject* gameObject, const tstring& typeName = TEXT("SkeletalMeshRenderer"));
        ~SkeletalMeshRenderer() override;

        void Render() override;

    protected:
        ComponentBase* CreateInstance_Impl(GameObject* _owner) override;
        ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;
        void CloneConvertAddress_Impl(AddressConvertMap* _map) override;

    public:
        Resource<SkeletalMesh> GetMesh() const { return m_Mesh; }
        std::vector<Resource<Material>> GetMaterialList() const { return m_MaterialList; }

        void SetMesh(Resource<SkeletalMesh> _mesh) { m_Mesh = _mesh; }
        void SetMaterial(std::vector<Resource<Material>> const _material) { m_MaterialList = _material; }

        void AddBone(const Bone& _bone) { m_Bones.push_back(_bone); }

    private:
        std::vector<Bone> m_Bones;

        Resource<SkeletalMesh> m_Mesh;
        std::vector<Resource<Material>> m_MaterialList;
    };
}
