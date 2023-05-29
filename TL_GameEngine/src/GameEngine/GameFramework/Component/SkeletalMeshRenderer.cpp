#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/SkeletalMeshRenderer.h"
#include "GameEngine/GameFramework/Component/Transform.h"

#include "Graphics/RenderSystem.h"
#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
    SkeletalMeshRenderer::SkeletalMeshRenderer(class GameObject* gameObject, const tstring& typeName /*= TEXT("SkeletalMeshRenderer")*/)
        : ComponentBase(gameObject, typeName)
          , IRenderableComponent(this) { }

    SkeletalMeshRenderer::~SkeletalMeshRenderer() { }

    void SkeletalMeshRenderer::Render()
    {
        if (m_bEnable == false ||
            GetGameObject()->GetActiveInHierarchy() == false ||
            GetGameObject()->GetEnable() == false)
            return;

        const auto _renderSystem = GameApplication::Instance->GetRenderSystem();
        const auto _transform = GetGameObject()->GetComponent<Transform>();
        auto _worldTm = _transform->GetWorldTM();

        struct PerObjectBuffer
        {
            TL_Math::Matrix _world;
            TL_Math::Matrix _worldInvTrans;
        };

        PerObjectBuffer _perObjectData;

        _perObjectData._world = _worldTm;
        _perObjectData._worldInvTrans = _worldTm.Invert().Transpose();

        struct PerSkeletal
        {
            TL_Math::Matrix _bone[200];
        };

        PerSkeletal _perSkelData;

        for (int i = 0; i < m_Bones.size(); ++i)
        {
            const Matrix& _boneOffsetTM = m_Bones[i].offsetTM;
            const Matrix _boneWorldTM = m_Bones[i].go->m_Transform.GetWorldTM();
            const Matrix _matrix = _boneOffsetTM * _boneWorldTM;
            _perSkelData._bone[i] = _matrix;
        }

        static TL_Graphics::IConstantBuffer* _staticBuffer = _renderSystem->CreateConstantBuffer(&_worldTm, sizeof(PerObjectBuffer));
        _staticBuffer->Update(&_perObjectData, sizeof(PerObjectBuffer));
        _staticBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

        static TL_Graphics::IConstantBuffer* _skelBuffer = _renderSystem->CreateConstantBuffer(&_worldTm, sizeof(PerSkeletal));
        _skelBuffer->Update(&_perSkelData, sizeof(PerSkeletal));
        _skelBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 2);

        m_Mesh.GetPtr()->GetMesh()->Set();

        auto& _subMeshList = m_Mesh->GetSubMeshList();
        for (int i = 0; i < _subMeshList.size(); ++i)
        {
            const auto _subMesh = _subMeshList[i];
            const int _subMeshStartIndex = _subMesh.first;
            const int _subMeshEndIndex =
                i == _subMeshList.size() - 1
                ? m_Mesh->GetIndexList().size()
                : m_Mesh->GetSubMeshList()[i + 1].first;
            const int _subMeshMaterialIndex = _subMesh.second;
            const uint _subMeshIndexCount = _subMeshEndIndex - _subMeshStartIndex;

            if (_subMeshMaterialIndex != -1 && _subMeshMaterialIndex < m_MaterialList.size())
            {
                const auto _material = m_MaterialList[_subMeshMaterialIndex].GetPtr();
                _material->Set();
            }

            _renderSystem->Draw(_subMeshIndexCount, _subMeshStartIndex);
        }
    }

    ComponentBase* SkeletalMeshRenderer::CreateInstance_Impl(GameObject* _owner)
    {
        return new SkeletalMeshRenderer(_owner);
    }

    ComponentBase* SkeletalMeshRenderer::CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map)
    {
        ComponentBase::CloneInstance_Impl(_cloned, _map);

        SkeletalMeshRenderer* _clonedRenderer = dynamic_cast<SkeletalMeshRenderer*>(_cloned);
        assert(_clonedRenderer != nullptr);

        _clonedRenderer->m_Mesh = m_Mesh;
        _clonedRenderer->m_MaterialList = m_MaterialList;
        _clonedRenderer->m_Bones = m_Bones;

        return _clonedRenderer;
    }

    void SkeletalMeshRenderer::CloneConvertAddress_Impl(AddressConvertMap* _map)
    {
        for(auto& _bone : m_Bones)
        {
            _bone.go = _map->GetMappedTarget<GameObject>(_bone.go);
        }
    }
}
