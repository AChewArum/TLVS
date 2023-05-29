#pragma once

#include "Common.h"
#include "SoundLib\SoundInfo.h"
//#include "SoundLib\FMODAPI.h"

namespace FMOD
{
	class Sound;
}

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC Sound
	{
	public:
		Sound(const std::string& filePath, eFileType fileType);
		virtual ~Sound();

		virtual void Release();
		
		virtual float GetLength();

		virtual FMOD::Sound* GetSound();

	private:
		FMOD::Sound* m_pSound;
	};
}