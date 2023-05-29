#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\AudioListener.h"
#include "GameEngine\GameFramework\Component\Transform.h"
#include "GameEngine\GameFramework\GameObject.h"

#include "GameEngine\GameFramework\Sound\AudioSystem.h"

#include "Math\TL_Math.h"

namespace TL_GameEngine
{
	AudioListener::AudioListener(class GameObject* _gameObject, const tstring& _typeName /*= TEXT("AudioListener")*/)
		: ComponentBase(_gameObject, _typeName)
		, m_eAudioVelocityUpdateMode(eAudioVelocityUpdateMode::Auto)
		, m_PrevPosiiton(TL_Math::Vector3::Zero)
	{

	}

	AudioListener::~AudioListener()
	{
	}

	void AudioListener::PreTick()
	{

	}

	void AudioListener::PostTick()
	{

	}

	void AudioListener::TryTick()
	{

	}

	void AudioListener::Tick()
	{

	}

	void AudioListener::OnStart()
	{
		AudioSystem::GetInstance()->SetListener(this);
	}

	void AudioListener::OnChangeEnable(bool _newEnable)
	{

	}
}