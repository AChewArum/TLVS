#pragma once

#include "Common_min.h"
#include "Graphics/RenderSystem.h"
#include "GameEngine/internal/dll.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API Material
    {
        friend class Resources;

    public:
        Material() :
            m_Material(nullptr),
            m_EmissiveStrength(0) { }

        ~Material()
        {
            TL_Graphics::RenderSystem::Get()->Return(m_Material);
        }

        void Set();

        void SetAlbedo(const class Texture* texture) {}
        void SetMetallic(const class Texture* texture) {}
        void SetRoughness(const class Texture* texture) {}

        void SetEmmisiveStrength(float _value) { m_EmissiveStrength = _value; }

        TL_Graphics::IMaterial* m_Material;
        float m_EmissiveStrength;

    public:
        auto GetMaterial() const { return m_Material; }
    };
}
