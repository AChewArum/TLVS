#pragma once

#include "Common_min.h"
#include "Graphics/RenderSystem.h"

namespace TL_GameEngine
{
    /*class ObjectInfo
    {
        Node* parent;
        std::vector<Node*> childs;

        std::string name;
    };

    class MeshObjectInfo : ObjectInfo
    {
		int matidx;
		int meshidx;
    };

    struct key
    {
        int frame;
        vector3 pos;
        vector3 rot;
        vector3 scale;
    };

    strcut animaionclipinfo
    {
        std::string name;
        vector<key>
    }

    class FBXPrefab
    {
        vector<MaterialInfo>;
        vector<StaticMeshInfo>;
        vector<animaionclipinfo>;

        ObjectInfo* root;
    };

    class FBX
    {
        std::vector<class Material>;
        class StaticMesh;
    };*/

    class StaticMesh
    {
        friend class Resources;

    public:
        StaticMesh() {}

        StaticMesh(const std::vector<TL_Common::StaticVertex>& _vertexList
                   , const std::vector<uint>& _indexList
                   , const std::vector<std::pair<uint, int>>& _subMeshList
                   , TL_Graphics::IMesh* _mesh
        ) :
            m_VertexList(_vertexList)
            , m_IndexList(_indexList)
            , m_SubMeshList(_subMeshList)
            , m_Mesh(_mesh) { }

        ~StaticMesh()
        {
            m_VertexList.clear();
            m_IndexList.clear();
            TL_Graphics::RenderSystem::Get()->Return(m_Mesh);
        }

        // private:
        std::vector<TL_Common::StaticVertex> m_VertexList;
        std::vector<uint> m_IndexList;
        std::vector<std::pair<uint, int>> m_SubMeshList; // pair<sub_mesh_end_index, material_index>
        TL_Graphics::IMesh* m_Mesh;

    public:
        auto& GetVertexList() const { return m_VertexList; }
        auto& GetIndexList() const { return m_IndexList; }
        auto& GetSubMeshList() const { return m_SubMeshList; }
        auto GetMesh() const { return m_Mesh; }
    };

    class SkeletalMesh
    {
        friend class Resources;

    public:
        SkeletalMesh() {}

        SkeletalMesh(const std::vector<TL_Common::SkeletalVertex>& _vertexList
                     , const std::vector<uint>& _indexList
                     , const std::vector<std::pair<uint, int>>& _subMeshList
                     , TL_Graphics::IMesh* _mesh
        ) :
            m_VertexList(_vertexList)
            , m_IndexList(_indexList)
            , m_SubMeshList(_subMeshList)
            , m_Mesh(_mesh) { }

        ~SkeletalMesh()
        {
            m_VertexList.clear();
            m_IndexList.clear();
            TL_Graphics::RenderSystem::Get()->Return(m_Mesh);
        }

    private:
        std::vector<TL_Common::SkeletalVertex> m_VertexList;
        std::vector<uint> m_IndexList;
        std::vector<std::pair<uint, int>> m_SubMeshList; // pair<submesh_end_index, material_index>
        TL_Graphics::IMesh* m_Mesh;

    public:
        auto& GetVertexList() const { return m_VertexList; }
        auto& GetIndexList() const { return m_IndexList; }
        auto& GetSubMeshList() const { return m_SubMeshList; }
        auto GetMesh() const { return m_Mesh; }
    };
}
