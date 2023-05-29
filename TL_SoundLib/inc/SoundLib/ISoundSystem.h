#pragma once

#include "Common.h"

#include "SoundLib\SoundInfo.h"

#include <string>

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC ISoundSystem abstract
	{
	public:
		virtual bool Initialize(uint32 maxChanelCnt) abstract;
		virtual void Release() abstract;

		//virtual bool LoadFile(const tstring& filePath, eFileType fileType) abstract;
		//virtual bool LoadFile(const std::string& filePath, eFileType fileType) abstract;
		virtual bool Tick(float deltaTime) abstract;
		virtual void Set3DSetting(float dopplerscale = 1, float distancefactor = 1, float rooloffscale = 1) abstract;

		virtual void SetListenerAttributes(ListenerAttributes& attributes) abstract;

		virtual SoundSource* CreateSoundSource() abstract;
		//virtual SoundListener* CreateSoundListener() abstract;
		virtual Sound* CreateSound(const tstring& filePath, eFileType fileType) abstract;
		virtual Sound* CreateSound(const std::string& filePath, eFileType fileType) abstract;
		virtual Effect* CreateEffect() abstract;
		virtual ReverbZones* CreateReverbZones() abstract;

		virtual void DeleteSoundSource(SoundSource*) abstract;
		//virtual void DeleteSoundListener(SoundListener*) abstract;
		virtual void DeleteSound(Sound*) abstract;
		virtual void DeleteEffect(Effect*) abstract;
		virtual void DeleteReverbZones(ReverbZones*) abstract;
	};

	extern "C"
	{
		SOUND_DLL_DECLSPEC ISoundSystem* SoundSystemInstance();
		SOUND_DLL_DECLSPEC void SoundSystemRelease();

		using SoundSystemConstructor = ISoundSystem * (*)();
		using SoundSystemDestructor = void (*)();
	}
}