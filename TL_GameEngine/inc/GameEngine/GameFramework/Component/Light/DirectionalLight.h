#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Graphics/ILight.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API DirectionalLight
        : public ComponentBase
    {
    public:
        DirectionalLight(GameObject* _gameObject, const tstring& _typeName = TEXT("DirectionalLight"));

        void Tick() override;
        
        void SetColor(const TL_Math::Vector3& _color) { m_Light.color = _color; }
        void SetIntensity(float _intensity) { m_Light.intensity = _intensity; }

    protected:
        TL_Graphics::DirectionalLight m_Light;
    };
}
