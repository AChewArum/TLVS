#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/Light/PointLight.h"

#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
    PointLight::PointLight(GameObject* _gameObject, const tstring& _typeName)
        : ComponentBase(_gameObject, _typeName)
    {
        m_Light.position = GetGameObject()->m_Transform.GetWorldPosition();
        m_Light.color = Vector3{ 1.0f, 1.0f, 1.0f };
        m_Light.intensity = 1.0f;
        m_Light.range = 10.0f;
    }

    void PointLight::Tick()
    {
        ComponentBase::Tick();

        m_Light.position = GetGameObject()->m_Transform.GetWorldPosition();

        const auto _renderSystem = GameApplication::Instance->GetRenderSystem();
        _renderSystem->SetLight(&m_Light);
    }
}
