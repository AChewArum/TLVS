#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Graphics/ILight.h"

namespace TL_GameEngine
{
    class PointLight
        : public ComponentBase
    {
    public:
        explicit PointLight(GameObject* _gameObject, const tstring& _typeName = TEXT("PointLight"));

        void Tick() override;

        void SetColor(const TL_Math::Vector3& _color) { m_Light.color = _color; }
        void SetIntensity(float _intensity) { m_Light.intensity = _intensity; }
        void SetRange(float _range) { m_Light.range = _range; }
        void SetAttenuation(const TL_Math::Vector3& _attenuation) { m_Light.attenuation = _attenuation; }

    protected:
        TL_Graphics::PointLight m_Light;
    };
}
