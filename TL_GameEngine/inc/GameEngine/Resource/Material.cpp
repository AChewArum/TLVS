#include "GameEngine_pch.h"
#include "Material.h"

#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
    void Material::Set()
    {
        m_Material->Set();

        struct ConstantBuffer_Material
        {
            float lodLevel;
            float emissiveStrength;
            float pad[2];
        } _buffer;

        _buffer.lodLevel = 0;
        _buffer.emissiveStrength = m_EmissiveStrength;

        static TL_Graphics::IConstantBuffer* _cb = GameApplication::Instance->GetRenderSystem()->CreateConstantBuffer(&_buffer, sizeof(ConstantBuffer_Material));
        _cb->Update(&_buffer, sizeof(ConstantBuffer_Material));
        _cb->Set(TL_Graphics::E_SHADER_TYPE::VS, 12);
    }
}
