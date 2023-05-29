#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/Light/DirectionalLight.h"

#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
    DirectionalLight::DirectionalLight(GameObject* _gameObject, const tstring& _typeName)
        : ComponentBase(_gameObject, _typeName)
    {
        m_Light.direction = TL_Math::Vector3{1, -1, 0};
        m_Light.color = TL_Math::Vector3{1, 1, 1};
        m_Light.intensity = 1.0f;
    }

    void DirectionalLight::Tick()
    {
        ComponentBase::Tick();

        //m_Light.direction = GetGameObject()->m_Transform.GetForwardDirection();

        const auto _renderSystem = GameApplication::Instance->GetRenderSystem();
        _renderSystem->SetLight(&m_Light);
    }
}
