#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/StaticMeshRenderer.h"

#include "Graphics/RenderSystem.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
    __declspec(align(16)) struct DirectionalLight
    {
        Vector3 direction;
    };

    void StaticMeshRenderer::Render()
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
        };

        PerObjectBuffer _perObjectData;
        _perObjectData._world = _worldTm;

        static TL_Graphics::IConstantBuffer* _worldBuffer = _renderSystem->CreateConstantBuffer(&_worldTm, sizeof(PerObjectBuffer));
        _worldBuffer->Update(&_perObjectData, sizeof(PerObjectBuffer));
        _worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

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

    ComponentBase* StaticMeshRenderer::CreateInstance_Impl(GameObject* _owner)
    {
        return new StaticMeshRenderer(_owner);
    }

    ComponentBase* StaticMeshRenderer::CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map)
    {
        ComponentBase::CloneInstance_Impl(_cloned, _map);

        StaticMeshRenderer* _clonedRenderer = dynamic_cast<StaticMeshRenderer*>(_cloned);
        assert(_clonedRenderer != nullptr);

        _clonedRenderer->m_Mesh = m_Mesh;
        _clonedRenderer->m_MaterialList = m_MaterialList;

        return _clonedRenderer;
    }
}
