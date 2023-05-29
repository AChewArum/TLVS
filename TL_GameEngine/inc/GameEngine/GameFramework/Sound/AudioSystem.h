#pragma once 

#include "Common.h"

#include "GameEngine\internal\dll.h"

#include "SoundLib\SoundInfo.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API AudioSystem
	{
		DECLARE_SINGLETON_CLASS(AudioSystem)

	public:
		void Initialize();

		void Release();

		void Tick(float deltaTime);

		void Pause(bool flag);

		void SetVolume(float volume);

		void SetListener(class AudioListener* listener);
		void DeleteListener();

		TL_Sound::SoundSource* CreateSoundSource();
		//TL_Sound::SoundListener* CreateSoundListener();
		TL_Sound::Sound* CreateSound(const tstring& filePath, TL_Sound::eFileType fileType);
		TL_Sound::Effect* CreateEffect();
		TL_Sound::ReverbZones* CreateReverbZones();

		void DeleteSoundSource(TL_Sound::SoundSource* soundSource);
		//void DeleteSoundListener();
		void DeleteSound(TL_Sound::Sound* sound);
		void DeleteEffect(TL_Sound::Effect* effect);
		void DeleteReverbZones(TL_Sound::ReverbZones*);
	private:
		bool LoadSoundDLL();

		float m_fMasterVolume;
		bool m_bPause;

		AudioListener* m_pListener;
		TL_Sound::ISoundSystem* m_pSoundSystem;
	};
}