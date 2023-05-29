#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Sound\AudioSystem.h"
#include "GameEngine\GameFramework\Sound\AudioClip.h"
#include "GameEngine\GameFramework\Component\AudioListener.h"
#include "GameEngine\GameFramework\Component\AudioSource.h"
#include "GameEngine\GameFramework\Component\Transform.h"
#include "GameEngine\GameFramework\GameObject.h"

#include "SoundLib\ISoundSystem.h"


TL_Sound::SoundSystemConstructor soundSytemConstructor;
TL_Sound::SoundSystemDestructor soundSytemDestructor;
HMODULE g_soundDLL;

namespace TL_GameEngine
{
	DEFINE_SINGLETON_CLASS(
		AudioSystem,
		[](){},
		[](){}
	);

	void AudioSystem::Initialize()
	{
		if (!LoadSoundDLL())
			AssertMessageBox(false, "Physics DLL load failed");

		m_pSoundSystem = soundSytemConstructor();

		m_pSoundSystem->Initialize(100);
	}

	void AudioSystem::Release()
	{
		soundSytemDestructor();
		FreeLibrary(g_soundDLL);

		m_pSoundSystem = nullptr;
	}

	void AudioSystem::Tick(float deltaTime)
	{
		if (m_pListener == nullptr)
		{
			return;
		}

		TL_Math::Matrix _tm = m_pListener->GetGameObject()->GetComponent<Transform>()->GetWorldTM();

		TL_Sound::ListenerAttributes attributes;

		attributes._position = _tm.Translation();
		attributes._forward = _tm.Forward();
		attributes._up = _tm.Up();
		attributes._velocity = TL_Math::Vector3::Zero;

		m_pSoundSystem->SetListenerAttributes(attributes);

		m_pSoundSystem->Tick(deltaTime);
	}

	void AudioSystem::Pause(bool flag)
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");
	}

	void AudioSystem::SetVolume(float volume)
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");
	}

	void AudioSystem::SetListener(class AudioListener* listener)
	{
		m_pListener = listener;
	}

	void AudioSystem::DeleteListener()
	{
		m_pListener = nullptr;
	}

	TL_Sound::SoundSource* AudioSystem::CreateSoundSource()
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");

		return m_pSoundSystem->CreateSoundSource();
	}

	/*TL_Sound::SoundListener* AudioSystem::CreateSoundListener()
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");

		return nullptr;
	}*/

	TL_Sound::Sound* AudioSystem::CreateSound(const tstring& filePath, TL_Sound::eFileType fileType)
	{
		//AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");

		return m_pSoundSystem->CreateSound(filePath, fileType);
	}

	TL_Sound::Effect* AudioSystem::CreateEffect()
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");

		return m_pSoundSystem->CreateEffect();
	}

	TL_Sound::ReverbZones* AudioSystem::CreateReverbZones()
	{
		AssertMessageBox(m_pSoundSystem != nullptr, "SoundSystem is null");

		return m_pSoundSystem->CreateReverbZones();
	}

	void AudioSystem::DeleteSoundSource(TL_Sound::SoundSource* soundSource)
	{
		m_pSoundSystem->DeleteSoundSource(soundSource);
	}

	void AudioSystem::DeleteSound(TL_Sound::Sound* sound)
	{
		m_pSoundSystem->DeleteSound(sound);
	}

	void AudioSystem::DeleteEffect(TL_Sound::Effect* effect)
	{
		m_pSoundSystem->DeleteEffect(effect);
	}

	void AudioSystem::DeleteReverbZones(TL_Sound::ReverbZones* reverbZones)
	{
		m_pSoundSystem->DeleteReverbZones(reverbZones);
	}

	bool AudioSystem::LoadSoundDLL()
	{
		g_soundDLL = LoadLibrary(_T("TL_SoundLib.dll"));

		AssertMessageBox(g_soundDLL != nullptr, "Load sound dll failed !");

		soundSytemConstructor = (TL_Sound::SoundSystemConstructor)GetProcAddress(g_soundDLL, "SoundSystemInstance");

		AssertMessageBox(soundSytemConstructor != NULL, "Load sound dll failed !");

		soundSytemDestructor = (TL_Sound::SoundSystemDestructor)GetProcAddress(g_soundDLL, "SoundSystemRelease");

		AssertMessageBox(soundSytemDestructor != NULL, "Load sound dll failed !");

		return true;
	}

}