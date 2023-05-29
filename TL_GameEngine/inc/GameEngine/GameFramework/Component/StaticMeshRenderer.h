#pragma once

#include "ComponentBase.h"
#include "GameEngine/Resource/Resource.h"

namespace TL_Graphics
{
    class IMesh;
    class IMaterial;
}

namespace TL_GameEngine
{
    class GAME_ENGINE_API StaticMeshRenderer :
        public ComponentBase,
        public IRenderableComponent
    {
    public:
        StaticMeshRenderer(GameObject* _gameObject, const tstring& _typeName = TEXT("StaticMeshRenderer"))
            : ComponentBase(_gameObject, _typeName),
              IRenderableComponent(this) {}

        void Render() override;

    protected:
        ComponentBase* CreateInstance_Impl(GameObject* _owner) override;
        ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;

    private:
        Resource<StaticMesh> m_Mesh;
        std::vector<Resource<Material>> m_MaterialList;

    public:
        Resource<StaticMesh> GetMesh() const { return m_Mesh; }
        std::vector<Resource<Material>> GetMaterialList() const { return m_MaterialList; }
        void SetMesh(Resource<StaticMesh> _mesh) { m_Mesh = _mesh; }
        void SetMaterial(std::vector<Resource<Material>> const _material) { m_MaterialList = _material; }
    };
}
