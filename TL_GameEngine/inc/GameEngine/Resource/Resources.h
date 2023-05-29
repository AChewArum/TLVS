#pragma once

#include "FBXObject.h"
#include "GameEngine/internal/dll.h"
#include "Common/SingletonInstance.h"
#include "Resource.h"
#include "Struct/VariousTypeMap.h"

namespace TL_FBXLibrary
{
    struct MeshInfo;
    struct FBXMaterialInfo;
    struct FBXPrefab;
    struct Animation;
}

namespace TL_GameEngine
{
    class GameObject;

    class GAME_ENGINE_API Resources
    {
        DECLARE_SINGLETON_CLASS(Resources)

    public:
        template <typename T>
        Resource<T> GetByPath(const tstring& _path)
        {
            return GetByPath_Impl<T>(_path);
        }

        static const tstring& GetResourcesRootPath();
        static tstring GetResourcePathByName(const tstring& _path);
        static bool IsFileOrDirectoryExist(const tstring& _path);
        static tstring GetDirectoryPath(const tstring& _path);
        static tstring GetConcretedPath(const std::vector<tstring>& _pathElems);

        // 나중에 마음에 드는 이름으로 바꿀것.
        GameObject* LoadFBXPrefab(class Scene* _scene, const tstring& _path);

    private:
        template <typename T>
        Resource<T> GetByPath_Impl(const tstring& _path)
        {
            static_assert(false, "need template specialization for T");
            return nullptr;
        }

        template <typename T>
        Resource<T> LoadByPath_Impl(const tstring& _path)
        {
            static_assert(false, "need template specialization for T");
            return nullptr;
        }

        template <>
        Resource<FBXObject> GetByPath_Impl(const tstring& _path);

        template <>
        Resource<FBXObject> LoadByPath_Impl(const tstring& _path);

        template <>
        Resource<Material> GetByPath_Impl(const tstring& path);

        template <>
        Resource<Material> LoadByPath_Impl(const tstring& path);

        template <>
        Resource<Texture> GetByPath_Impl(const tstring& path);

        template <>
        Resource<Texture> LoadByPath_Impl(const tstring& path);

        template <>
        Resource<AnimationClip> GetByPath_Impl(const tstring& path);

        template <>
        Resource<AnimationClip> LoadByPath_Impl(const tstring& path);

    private:
        // 임시 짬통들
        Resource<StaticMesh> MakeStaticMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo);
        Resource<SkeletalMesh> MakeSkeletalMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo);
        Resource<Material> MakeMaterialFromFBXMesh(const tstring& fbxFilePath, TL_FBXLibrary::FBXMaterialInfo& materialInfo);
        Resource<AnimationClip> MakeAnimationClipFromFBXMesh(TL_FBXLibrary::Animation& animation);

        tstring m_ResourcesRootPath;
        std::map<tstring, tstring> m_ResourceNamePathMap;
        std::map<tstring, Resource<FBXObject>> m_FBXObjectList;
        std::map<tstring, Resource<StaticMesh>> m_StaticMeshList;
        std::map<tstring, Resource<Material>> m_MaterialList;
        std::map<tstring, Resource<Texture>> m_TextureList;

        Resource<StaticMesh> m_DefaultCubeMesh;
        Resource<Material> m_DefaultMaterial;

    public:
        Resource<StaticMesh> GetDefaultCubeMesh();
        Resource<Material> GetDefaultMaterial();
    };
}
