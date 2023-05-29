#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Sound\AudioClip.h"
#include "GameEngine\GameFramework\Sound\AudioSystem.h"

#include "SoundLib\Metal\Sound.h"

namespace TL_GameEngine
{
	AudioClip::AudioClip()
	{

	}

	AudioClip::~AudioClip()
	{
		if (m_pSound != nullptr)
		{
			AudioSystem::GetInstance()->DeleteSound(m_pSound);
			m_pSound = nullptr;
		}
	}

	bool AudioClip::GetData()
	{
		return false;
	}


	TL_GameEngine::AudioClip* AudioClip::Create(tstring& path, TL_Sound::eFileType type)
	{
		AudioClip* _newAudioClip = new AudioClip();

		TL_Sound::Sound* _newSound = AudioSystem::GetInstance()->CreateSound(path, type);

		_newAudioClip->m_pSound = _newSound;

		return _newAudioClip;
	}

}
