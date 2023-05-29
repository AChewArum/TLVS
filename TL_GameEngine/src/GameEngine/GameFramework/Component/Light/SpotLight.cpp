#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/Light/SpotLight.h"

#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
	SpotLight::SpotLight(GameObject* _gameObject, const tstring& _typeName)
		: ComponentBase(_gameObject, _typeName)
	{
		m_Light.position = GetGameObject()->m_Transform.GetWorldPosition();
		m_Light.color = Vector3{ 1.0f, 0.0f, 1.0f };
		m_Light.intensity = 10.0f;
		m_Light.range = 10.0f;
		m_Light.direction = Vector3::Down;
		m_Light.spot = 10.0f;
	}

	void SpotLight::Tick()
	{
		ComponentBase::Tick();

		auto& _transform = GetGameObject()->m_Transform;

		m_Light.position = _transform.GetWorldPosition();
		m_Light.direction = Vector3::Transform(Vector3::Forward, _transform.GetWorldRotation());


		const auto _renderSystem = GameApplication::Instance->GetRenderSystem();
		_renderSystem->SetLight(&m_Light);
	}
}
