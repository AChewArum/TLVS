#include "GameEngine_pch.h"
#include "GameEngine/Resource/Resources.h"

#include <filesystem>

#include "FBXLibrary/FBXModelLoader.h"
#include "Struct/Vertex.h"
#include "Graphics/IVertex.h"
#include "Graphics/RenderSystem.h"

#include "Common_min.h"

// 임시 나중에 분리하고싶으면 할것
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/GameFramework/Component/Animation.h"
#include "GameEngine/Gameframework/Component/Animator.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"
#include "GameEngine/GameFramework/Component/SkeletalMeshRenderer.h"

using namespace TL_Math;
using namespace TL_Common;

namespace TL_GameEngine
{
    DEFINE_SINGLETON_CLASS(
        Resources,
        [](){},
        [](){}
    )

    const tstring& Resources::GetResourcesRootPath()
    {
        static struct __temp
        {
            tstring rootPath;

            __temp()
            {
                if (const char* _value = std::getenv("TL_PROJECT_RESOURCES_ROOT"))
                    rootPath = StringHelper::ToTString(_value);
                else
                    rootPath = TEXT("./");

                Instance->m_ResourcesRootPath = rootPath;
            }
        } _temp;

        return _temp.rootPath;
    }

    tstring Resources::GetResourcePathByName(const tstring& _path)
    {
        static struct __temp
        {
            std::map<tstring, tstring> resourceNamePathMap;

            __temp()
            {
                auto _iter = std::filesystem::recursive_directory_iterator{std::filesystem::path{GetResourcesRootPath()}};
                for (const auto& _entry : _iter)
                {
                    if (_entry.is_regular_file() || _entry.is_block_file() || _entry.is_character_file())
                    {
                        resourceNamePathMap.insert({_entry.path().filename(), _entry.path()});
                    }
                }

                Instance->m_ResourceNamePathMap = resourceNamePathMap;
            }
        } _temp;

        const auto _iter = _temp.resourceNamePathMap.find(_path);
        if (_iter == _temp.resourceNamePathMap.end())
            return TEXT("");

        return _iter->second;
    }

    bool Resources::IsFileOrDirectoryExist(const tstring& _path)
    {
        const std::filesystem::path _fsPath{_path};
        return std::filesystem::exists(_fsPath);
    }

    tstring Resources::GetDirectoryPath(const tstring& _path)
    {
        const std::filesystem::path _fsPath{_path};
        return _fsPath.parent_path();
    }

    tstring Resources::GetConcretedPath(const std::vector<tstring>& _pathElems)
    {
        std::filesystem::path _result;
        std::for_each(_pathElems.begin(), _pathElems.end(), [&_result](const tstring& e) { _result = _result / e; });
        return _result;
    }

    template <>
    Resource<FBXObject> Resources::GetByPath_Impl<FBXObject>(const tstring& _path)
    {
        const auto _absolutePath = GetConcretedPath({GetResourcesRootPath(), _path});
        const auto _iter = m_FBXObjectList.find(_absolutePath);

        if (_iter == m_FBXObjectList.end())
        {
            Resource<FBXObject> _outRes = LoadByPath_Impl<FBXObject>(_absolutePath);
            m_FBXObjectList.insert({_absolutePath, _outRes});
            return _outRes;
        }

        return _iter->second;
    }

    template <>
    Resource<FBXObject> Resources::LoadByPath_Impl<FBXObject>(const tstring& _path)
    {
        const tstring _absolutePath = GetConcretedPath({ GetResourcesRootPath(), _path });
        const tstring _fileName = std::filesystem::path{ _path }.filename();

        // FBX 파일을 불러옵니다.

        TL_FBXLibrary::FBXModelLoader _loader;
        bool _result = false;

        // FBX 파일 or Binary 파일인지 검사
        const bool _bLoadFromBinary = _path.rfind(TEXT(".TL")) != -1; // 이름에 .TL이 없으면 -1 반환
        const bool _bLoadFromFbx = _bLoadFromBinary == true
            ? false
            : _path.rfind(TEXT(".fbx")) != -1;

        if (_bLoadFromBinary)
        {
            _result = _loader.BinaryLoad(_absolutePath);
            assert(_result == true);
        }
        else if (_bLoadFromFbx)
        {
            _result = _loader.Init();
            assert(_result == true);

            _result = _loader.Load(_absolutePath);
            assert(_result == true);

            _loader.FbxConvertOptimize();
        }
        else
        {
            _loader.BinaryFolderLoad(_absolutePath);
        }

        auto* _prefab = _loader.GetPrefab();

        std::map<tstring, Resource<StaticMesh>> _loadStaticMeshMap;
        std::map<tstring, Resource<SkeletalMesh>> _loadSkeletalMeshMap;
        std::vector<Resource<Material>> _loadMaterials{ _prefab->m_MaterialList.size() };

        std::map<tstring, GameObject*> _nodeGOMap;
        std::vector<GameObject*> _boneGOList{ _prefab->m_BoneList.size() };
        GameObject* _armature = nullptr;

        GameObject* _root = GameObject::Create();
        _root->SetName(_fileName);


        // 메시 리소스들을 생성합니다.

        for (size_t i = 0; i < _prefab->m_MeshList.size(); i++)
        {
            if (_prefab->m_MeshList[i].isStatic == true)
            {
                auto _loadMesh = MakeStaticMeshFromFBXMesh(_prefab->m_MeshList[i]);
                _loadStaticMeshMap.insert(std::make_pair(_prefab->m_MeshList[i].meshNodeName, _loadMesh));
            }
            else
            {
                auto _loadMesh = MakeSkeletalMeshFromFBXMesh(_prefab->m_MeshList[i]);
                _loadSkeletalMeshMap.insert(std::make_pair(_prefab->m_MeshList[i].meshNodeName, _loadMesh));
            }
        }

        // 머티리얼 리소스들을 생성합니다.

        for (size_t i = 0; i < _prefab->m_MaterialList.size(); i++)
        {
            auto _loadMat = MakeMaterialFromFBXMesh(_absolutePath, _prefab->m_MaterialList[i]);
            _loadMaterials[i] = _loadMat;
        }

        if (_prefab->m_BoneList.empty() == false)
        {
            // 본이 존재할 때 실행됩니다.

            _armature = GameObject::Create();
            _armature->SetName(TEXT("Armature"));
            _armature->SetParent(_root);

            // TODO: 아래는 본이 정상적으로 생성/설정되는지 확인하기 위한 테스트 코드입니다.
            // 본의 위치를 가시적으로 확인하기 위해
            // 본에 대응하는, 눈에 보이는 게임 오브젝트를 생성합니다.

            for (size_t _boneIndex = 0; _boneIndex < _prefab->m_BoneList.size(); _boneIndex++)
            {
                // 본에 대응하는 게임 오브젝트를 생성합니다.

                GameObject* _boneGO = GameObject::Create();
                _boneGO->SetName(_prefab->m_BoneList[_boneIndex].boneName);
                _boneGOList[_boneIndex] = _boneGO;

                auto _boneRenderer = _boneGO->AddComponent<StaticMeshRenderer>();
                _boneRenderer->SetMesh(GetDefaultCubeMesh());
                _boneRenderer->SetMaterial({ GetDefaultMaterial() });

                // 본에 대응하는 게임 오브젝트간 계층 관계를 설정합니다.
                // 반드시 본의 계층 구조대로 게임 오브젝트가 생성되어 있어야 합니다.

                GameObject* _parent = _prefab->m_BoneList[_boneIndex].parentBoneIndex == -1
                    ? _armature
                    : _boneGOList[_prefab->m_BoneList[_boneIndex].parentBoneIndex];

                _boneGO->SetParent(_parent);

                _boneGO->m_Transform.SetWorldTM(_prefab->m_BoneList[_boneIndex].m_WorldTM);
            }

			_armature->m_Transform.SetLocalRotation(
                Quaternion::CreateFromAxisAngleDeg(Vector3::Right, -90.0f) 
                * Quaternion::CreateFromAxisAngleDeg(Vector3::Up, 180.0f));
        }

        if (_prefab->m_AnimList.empty() == false && _prefab->m_BoneList.empty() == false)
        {
            // 본 애니메이션이 있을 때 실행됩니다.

            std::map<tstring, Resource<AnimationClip>> _loadAnims;

            // 애니메이션 클립 리소스들을 생성합니다.

            for (size_t i = 0; i < _prefab->m_AnimList.size(); i++)
            {
                auto _loadAnim = MakeAnimationClipFromFBXMesh(_prefab->m_AnimList[i]);
                _loadAnims.insert(std::make_pair(_prefab->m_AnimList[i].name, _loadAnim));
            }

            //auto _animCom = _armature->AddComponent<Animation>();
            auto _animCom = _armature->AddComponent<Animator>();

            tstring name;
            for (auto _pair : _loadAnims)
            {
                name = _pair.first;
                _animCom->AddClip(_pair.second);
            }

            // TODO: 임시적으로 가장 첫 번째 애니메이션을 재생합니다.
            //_animCom->SetClip(_loadAnims.begin()->second);
            //_animCom->Play();
        }

        for (size_t i = 0; i < _prefab->m_ObjNodeList.size(); i++)
        {
            auto _meshNode = _prefab->m_ObjNodeList[i];

            // TODO: 3-3 스테이지를 부를때는 SetName를 NodeName으로 해야함.  
            GameObject* _newGO = GameObject::Create();
            _newGO->SetName(_meshNode.nodeName);
            _newGO->m_Transform.SetLocalTM(_meshNode.nodeTM);
        	_nodeGOMap.insert({ _meshNode.nodeName, _newGO });

            if (_meshNode.hasMesh == true)
            {
                // 이 mesh node에 대응하는 게임 오브젝트에
                // 적절한 static(또는 skeletal) mesh와 material을 설정합니다.

                // TODO: (논의 필요) 왜 meshList 내 노드인데 mesh를 가지고 있는지에 대한 여부를 판별해야 하나요?

                auto _iter = std::find_if(
                    _prefab->m_MeshList.begin(), _prefab->m_MeshList.end(),
                    [_meshNode](TL_FBXLibrary::MeshInfo _info) { return _info.meshNodeName == _meshNode.meshNodeName; });
                assert(_iter != _prefab->m_MeshList.end());
                auto _findMeshInfo = *_iter;

                if (_findMeshInfo.isStatic == true)
                {
                    auto _staticMeshIter = _loadStaticMeshMap.find(_findMeshInfo.meshNodeName);
                    assert(_staticMeshIter != _loadStaticMeshMap.end());

                    auto _renderer = _newGO->AddComponent<StaticMeshRenderer>();
                    _renderer->SetMesh(_staticMeshIter->second);

                    if (_findMeshInfo.isMaterial)
                    {
                        _renderer->SetMaterial(_loadMaterials);
                    }
                }
                else
                {
                    auto _skeletalMeshIter = _loadSkeletalMeshMap.find(_findMeshInfo.meshNodeName);
                    assert(_skeletalMeshIter != _loadSkeletalMeshMap.end());

                    auto _renderer = _newGO->AddComponent<SkeletalMeshRenderer>();
                    _renderer->SetMesh(_skeletalMeshIter->second);

                    if (_findMeshInfo.isMaterial)
                    {
                        _renderer->SetMaterial(_loadMaterials);
                    }

                    for (int i = 0; i < _prefab->m_BoneList.size(); ++i)
                    {
                        GameObject* const _boneGO = _boneGOList[i];
                        const Matrix& _boneOffsetTM = _prefab->m_BoneList[i].m_bone_From3DSceneSpaceToBoneSpaceTM;

                        // skeletal renderer에 본을 연결합니다.
                        _renderer->AddBone({ _boneGO, _boneOffsetTM });
                    }
                }

                // 게임 오브젝트의 회전을 조정해 축 변환을 합니다.
            /*    auto _originLocalRotation = _newGO->m_Transform.GetLocalRotation();
                _newGO->m_Transform.SetLocalRotation(_originLocalRotation * Quaternion::CreateFromAxisAngleDeg(Vector3::Right, -90.0f));*/
            }
        }

        for (int i = 0; i < _prefab->m_ObjNodeList.size(); ++i)
        {
            const auto& _mesh = _prefab->m_ObjNodeList[i];

            // 노드의 계층 구조에 알맞게
            // 노드에 대응하는 게임 오브젝트들의 계층 구조를 설정합니다.

            const auto _iter = _nodeGOMap.find(_mesh.parentNodeName);

            GameObject* _parent = _iter == _nodeGOMap.end() ? _root : _iter->second;
            _nodeGOMap[_mesh.nodeName]->SetParent(_parent);
        }

        _result = _loader.Release();
        assert(_result != false);

        // FBXObject를 만들어 반환합니다.
        FBXObject* _object = new FBXObject{ _root, _loadStaticMeshMap, _loadSkeletalMeshMap, _loadMaterials };
        Resource<FBXObject> _outRes;
        _outRes.SetPtr(_object);
        return _outRes;
    }

    template <>
    Resource<Material> Resources::GetByPath_Impl<Material>(const tstring& _path)
    {
        const auto _absolutePath = GetConcretedPath({GetResourcesRootPath(), _path});
        const auto _iter = m_MaterialList.find(_absolutePath);

        if (_iter == m_MaterialList.end())
        {
            Resource<Material> _outRes = LoadByPath_Impl<Material>(_absolutePath);
            m_MaterialList.insert({_absolutePath, _outRes});
            return _outRes;
        }

        return _iter->second;
    }

    template <>
    Resource<Material> Resources::LoadByPath_Impl<Material>(const tstring& _path)
    {
        // Todo :: 아직 스탠다드 머테리얼에 관해서 없음

        TL_Graphics::MaterialDesc matDesc;

        const auto _material = new Material();
        _material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

        Resource<Material> _outRes;
        _outRes.SetPtr(_material);

        return _outRes;
    }

    template <>
    Resource<Texture> Resources::GetByPath_Impl<Texture>(const tstring& _path)
    {
        const auto _absolutePath = GetConcretedPath({GetResourcesRootPath(), _path});
        const auto _iter = m_TextureList.find(_absolutePath);

        if (_iter == m_TextureList.end())
        {
            Resource<Texture> _outRes = LoadByPath_Impl<Texture>(_absolutePath);
            m_TextureList.insert({_absolutePath, _outRes});
            return _outRes;
        }

        return _iter->second;
    }

    template <>
    Resource<Texture> Resources::LoadByPath_Impl<Texture>(const tstring& _path)
    {
        auto _texture = new Texture();
        _texture->m_Texture = TL_Graphics::RenderSystem::Get()->CreateTexture(_path);

        Resource<Texture> _outRes;
        _outRes.SetPtr(_texture);

        return _outRes;
    }

    GameObject* Resources::LoadFBXPrefab(Scene* _scene, const tstring& _path)
    {
        auto _fbxObject = GetByPath<FBXObject>(_path);
        assert(_fbxObject != nullptr);

        auto _go = GameObject::Spawn(_scene, _fbxObject->GetPrefab());
        return _go;
    }

    Resource<TL_GameEngine::StaticMesh> Resources::MakeStaticMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo)
    {
        auto& _vertexList = meshInfo.StaticVertex;

        uint _allIndexCount = 0;
        std::for_each(meshInfo.indexBuffer.begin(), meshInfo.indexBuffer.end(),
                      [&_allIndexCount](const std::pair<std::vector<TL_FBXLibrary::MyFace>, uint>& _subMeshFaceList) { _allIndexCount = _allIndexCount + _subMeshFaceList.first.size() * 3; });

        std::vector<uint> _indexList;
        _indexList.resize(_allIndexCount);
        std::vector<std::pair<uint, int>> _subMeshList{meshInfo.indexBuffer.size()};

        int _lastSubMeshStartIndex = 0;
        for (int i = 0; i < meshInfo.indexBuffer.size(); ++i)
        {
            const auto& _subMeshFaceList = meshInfo.indexBuffer[i].first;
            const int _subMeshIndexCount = _subMeshFaceList.size() * 3;
            const int _subMeshMaterialIndex = meshInfo.indexBuffer[i].second;

            for (int j = 0; j < _subMeshFaceList.size(); ++j)
            {
                int _targetIndex = _lastSubMeshStartIndex + j * 3;
                _indexList[_targetIndex] = _subMeshFaceList[j].index[0];
                _indexList[_targetIndex + 1] = _subMeshFaceList[j].index[1];
                _indexList[_targetIndex + 2] = _subMeshFaceList[j].index[2];
            }

            _subMeshList[i] = std::make_pair(_lastSubMeshStartIndex, _subMeshMaterialIndex);
            _lastSubMeshStartIndex = _lastSubMeshStartIndex + _subMeshIndexCount;
        }

        // graphics mesh 리소스를 생성합니다.

        TL_Graphics::VertexAttribute vertexAttribute;
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");
        vertexAttribute.AddData(meshInfo.StaticVertex.data(), sizeof(StaticVertex) * meshInfo.StaticVertex.size());

        const auto _graphicsMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(
            vertexAttribute,
            _indexList.data(), _indexList.size(),
            TL_Graphics::E_MESH_TYPE::STATIC);

        // static mesh 인스턴스를 생성합니다.

        std::vector<StaticVertex> _vertexList2{_vertexList.size()};
        for (int i = 0; i < _vertexList2.size(); ++i)
        {
            memcpy(&_vertexList2[i].position, &_vertexList[i].pos, sizeof(Vector3));
            memcpy(&_vertexList2[i].uv, &_vertexList[i].uv, sizeof(Vector2));
            memcpy(&_vertexList2[i].normal, &_vertexList[i].normal, sizeof(Vector3));
        }

        auto _mesh = new StaticMesh{_vertexList2, _indexList, _subMeshList, _graphicsMesh};
        _mesh->m_Mesh = _graphicsMesh;

        Resource<StaticMesh> _outRes;
        _outRes.SetPtr(_mesh);

        return _outRes;
    }

    Resource<TL_GameEngine::SkeletalMesh> Resources::MakeSkeletalMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo)
    {
        auto& _vertexList = meshInfo.SkeletalVertex;

        uint _allIndexCount = 0;
        std::for_each(meshInfo.indexBuffer.begin(), meshInfo.indexBuffer.end(),
                      [&_allIndexCount](const std::pair<std::vector<TL_FBXLibrary::MyFace>, uint>& _subMeshFaceList) { _allIndexCount = _allIndexCount + _subMeshFaceList.first.size() * 3; });

        std::vector<uint> _indexList;
        _indexList.resize(_allIndexCount);
        std::vector<std::pair<uint, int>> _subMeshList{meshInfo.indexBuffer.size()};

        int _lastSubMeshStartIndex = 0;
        for (int i = 0; i < meshInfo.indexBuffer.size(); ++i)
        {
            const auto& _subMeshFaceList = meshInfo.indexBuffer[i].first;
            const int _subMeshIndexCount = _subMeshFaceList.size() * 3;
            const int _subMeshMaterialIndex = meshInfo.indexBuffer[i].second;

            for (int j = 0; j < _subMeshFaceList.size(); ++j)
            {
                int _targetIndex = _lastSubMeshStartIndex + j * 3;
                _indexList[_targetIndex] = _subMeshFaceList[j].index[0];
                _indexList[_targetIndex + 1] = _subMeshFaceList[j].index[1];
                _indexList[_targetIndex + 2] = _subMeshFaceList[j].index[2];
            }

            _subMeshList[i] = std::make_pair(_lastSubMeshStartIndex, _subMeshMaterialIndex);
            _lastSubMeshStartIndex = _lastSubMeshStartIndex + _subMeshIndexCount;
        }

        // graphics mesh 리소스를 생성합니다.

        TL_Graphics::VertexAttribute vertexAttribute;
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::index[0]) * 4, TL_Graphics::DataType::UINT, "BONE_INDEX");
        vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::weight1), TL_Graphics::DataType::FLOAT, "BONE_WEIGHT");
        //vertexAttribute.AddElementToDesc(sizeof(SkeletalVertex::weight2), TL_Graphics::DataType::FLOAT, "BONE_WEIGHT");
        vertexAttribute.AddData(meshInfo.SkeletalVertex.data(), sizeof(SkeletalVertex) * meshInfo.SkeletalVertex.size());

        const auto _graphicsMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(
            vertexAttribute,
            _indexList.data(), (UINT)_indexList.size(),
            TL_Graphics::E_MESH_TYPE::SKINNING);

        // skeletal mesh 인스턴스를 생성합니다.

        std::vector<SkeletalVertex> _vertexList2{_vertexList.size()};
        for (int i = 0; i < _vertexList2.size(); ++i)
        {
            memcpy(&_vertexList2[i].position, &_vertexList[i].pos, sizeof(Vector3));
            memcpy(&_vertexList2[i].uv, &_vertexList[i].uv, sizeof(Vector2));
            memcpy(&_vertexList2[i].normal, &_vertexList[i].normal, sizeof(Vector3));
            memcpy(&_vertexList2[i].tangent, &_vertexList[i].tangent, sizeof(Vector3));
            memcpy(&_vertexList2[i].bitangent, &_vertexList[i].bitangent, sizeof(Vector3));
            memcpy(&_vertexList2[i].index, &_vertexList[i].index, sizeof(int) * 4);
            memcpy(&_vertexList2[i].weight1, &_vertexList[i].weight1, sizeof(Vector3));
        }

        Resource<SkeletalMesh> _outRes;
        _outRes.SetPtr(new SkeletalMesh{_vertexList2, _indexList, _subMeshList, _graphicsMesh});

        return _outRes;
    }

    Resource<TL_GameEngine::Material> Resources::MakeMaterialFromFBXMesh(const tstring& fbxFilePath, TL_FBXLibrary::FBXMaterialInfo& materialInfo)
    {
        const tstring _fbxFileDirectory = GetDirectoryPath(fbxFilePath);
        TL_Graphics::MaterialDesc matDesc;

        // Todo : 머테리얼 종류에 따라 쉐이더 변경 하는 부분 필요함
        const auto _material = new Material();


        // fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
        if (materialInfo.baseColorFile.length() > 0)
        {
            auto _path = GetConcretedPath({_fbxFileDirectory, materialInfo.baseColorFile});


            if (IsFileOrDirectoryExist(_path))
            {
                matDesc.baseColor_opcityFilePath = _path;
                /*
                auto texture = GetByPath_Impl<Texture>(_path);
                _material->SetAlbedo(texture.GetPtr());*/
            }
            else
            {
                // 파일 경로를 읽지 못했음
            }
        }

        // fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
        if (materialInfo.normalMapFile.length() > 0)
        {
            auto _path = GetConcretedPath({_fbxFileDirectory, materialInfo.normalMapFile});


            if (IsFileOrDirectoryExist(_path))
            {
                matDesc.normalFilePath = _path;
                //auto texture = GetByPath_Impl<Texture>(_path);
                //_material->SetAlbedo(texture.GetPtr());
            }
            else
            {
                // 파일 경로를 읽지 못했음
            }
        }

        // fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
        if (materialInfo.roughnessMapFile.length() > 0)
        {
            auto _path = GetConcretedPath({_fbxFileDirectory, materialInfo.roughnessMapFile});


            if (IsFileOrDirectoryExist(_path))
            {
                matDesc.roughness_specular_metallic_AOFilePath = _path;

                /*auto texture = GetByPath_Impl<Texture>(_path);
                _material->SetMetallic(texture.GetPtr());
                _material->SetRoughness(texture.GetPtr());*/
            }
            else
            {
                // 파일 경로를 읽지 못했음
            }
        }
        
        if (materialInfo.emissiveFile.length() > 0)
        {
            auto _path = GetConcretedPath({ _fbxFileDirectory, materialInfo.emissiveFile });

            if (IsFileOrDirectoryExist(_path))
            {
                matDesc.emissiveFilePath = _path;
            }
        }


        _material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

        Resource<Material> _outRes;
        _outRes.SetPtr(_material);

        return _outRes;
    }

    Resource<TL_GameEngine::AnimationClip> Resources::MakeAnimationClipFromFBXMesh(TL_FBXLibrary::Animation& animation)
    {
        float _totalTime = (float)animation.totalKeyFrame / animation.frameRate;

        std::vector<AnimationSnap> _snapList{animation.keyFrameInfo.size()};
        for (size_t _keyIdx = 0; _keyIdx < animation.keyFrameInfo.size(); _keyIdx++)
        {
            AnimationSnap& _newSnap = _snapList[_keyIdx];

            for (size_t _snapIdx = 0; _snapIdx < animation.keyFrameInfo[_keyIdx].animInfo.size(); _snapIdx++)
            {
                Keyframe_Vector3 _posKey{animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].time, animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].pos};
                Keyframe_Quaternion _rotKey{animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].time, animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].rot};
                Keyframe_Vector3 _scaleKey{animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].time, animation.keyFrameInfo[_keyIdx].animInfo[_snapIdx].scl};

                _newSnap.AddPositionKeyFrame(_posKey);
                _newSnap.AddRotationKeyFrame(_rotKey);
                _newSnap.AddScaleKeyFrame(_scaleKey);
            }
            _newSnap.m_TargetName = animation.keyFrameInfo[_keyIdx].boneName;
            _newSnap.m_MaxFrameRate = _totalTime;
        }

        auto* _newClip = new AnimationClip(animation.name, std::move(_snapList));

        Resource<AnimationClip> _outRes;
        _outRes.SetPtr(_newClip);

        return _outRes;
    }

    Resource<StaticMesh> Resources::GetDefaultCubeMesh()
    {
        if (m_DefaultCubeMesh.GetPtr() != nullptr)
        {
            // 이미 기본 큐브 메시 리소스를 생성했다면
            // 기존의 리소스를 반환합니다.
            return m_DefaultCubeMesh;
        }

        // 그렇지 않다면 생성한 뒤 반환합니다.

        static const std::vector<StaticVertex> _vertexList({
            //앞면
            {{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}, {0, 0, 1}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0, 0, 1}, {-1, 0, 0}, {0, 1, 0}},
            {{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {0, 0, 1}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0, 0, 1}, {-1, 0, 0}, {0, 1, 0}},

            //뒷면
            {{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}},
            {{+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}},
            {{+0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0, 0, -1}, {1, 0, 0}, {0, -1, 0}},

            //왼면
            {{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}},

            //오른면
            {{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}},
            {{-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}},
            {{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}},

            //윗면
            {{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0, -1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {0, -1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0, -1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0, -1, 0}, {-1, 0, 0}, {0, 1, 0}},

            //아랫면
            {{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}, {0, 1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0, 1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{-0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {0, 1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {{+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0, 1, 0}, {-1, 0, 0}, {0, 1, 0}}
        });

        static const std::vector<uint> _indexList =
        {
            //front
            0, 1, 2,
            1, 3, 2,
            //back
            5, 4, 6,
            5, 6, 7,

            //left
            8, 10, 9,
            9, 10, 11,
            //right
            12, 14, 13,
            13, 14, 15,

            //up
            16, 17, 18,
            17, 19, 18,
            //down
            20, 22, 21,
            21, 22, 23
        };

        TL_Graphics::VertexAttribute vertexAttribute;
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
        vertexAttribute.AddElementToDesc(sizeof(StaticVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");
        vertexAttribute.AddData(const_cast<StaticVertex*>(_vertexList.data()), sizeof(StaticVertex) * _vertexList.size());

        auto _cubeMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(
            vertexAttribute,
            const_cast<uint*>(_indexList.data()), _indexList.size(),
            TL_Graphics::E_MESH_TYPE::STATIC
        );

        m_DefaultCubeMesh.SetPtr(
            new StaticMesh(_vertexList, _indexList, std::vector({std::pair<uint, int>(0, 0)}), _cubeMesh));
        return m_DefaultCubeMesh;
    }

    Resource<Material> Resources::GetDefaultMaterial()
    {
        if (m_DefaultMaterial.GetPtr() != nullptr)
        {
            // 이미 기본 큐브 머티리얼 리소스를 생성했다면
            // 기존의 리소스를 반환합니다.
            return m_DefaultMaterial;
        }

        // 그렇지 않다면 생성한 뒤 반환합니다.

        Material* _material = new Material();
        _material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

        m_DefaultMaterial.SetPtr(_material);
        return m_DefaultMaterial;
    }
}
