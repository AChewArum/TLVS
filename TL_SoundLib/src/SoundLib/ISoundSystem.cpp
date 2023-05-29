#include "SoundLib\SoundPCH.h"
#include "SoundLib\ISoundSystem.h"
#include "SoundLIB\SoundSystem.h"

namespace TL_Sound
{
	static ISoundSystem* g_pSoundSystem = nullptr;

	SOUND_DLL_DECLSPEC ISoundSystem* SoundSystemInstance()
	{
		if (g_pSoundSystem == nullptr)
		{
			g_pSoundSystem = SoundSystem::Instance();
		}

		return g_pSoundSystem;
	}

	SOUND_DLL_DECLSPEC void SoundSystemRelease()
	{
		g_pSoundSystem->Release();

		delete g_pSoundSystem;

		g_pSoundSystem = nullptr;
	}

}