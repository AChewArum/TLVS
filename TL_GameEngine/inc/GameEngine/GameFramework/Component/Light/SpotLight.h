#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Graphics/ILight.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API SpotLight
        : public ComponentBase
    {
    public:
        explicit SpotLight(GameObject* _gameObject, const tstring& _typeName = TEXT("SpotLight"));

        void Tick() override;

        void SetColor(const TL_Math::Vector3& _color) { m_Light.color = _color; }
        void SetIntensity(float _intensity) { m_Light.intensity = _intensity; }
        void SetRange(float _range) { m_Light.range = _range; }
        void SetAttenuation(const TL_Math::Vector3& _attenuation) { m_Light.attenuation = _attenuation; }
        void SetSpot(float _spot) { m_Light.spot = _spot; }

    protected:
        TL_Graphics::SpotLight m_Light;
    };
}
