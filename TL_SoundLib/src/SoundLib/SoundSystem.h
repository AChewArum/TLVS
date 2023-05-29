#pragma once

#include "Common.h"
#include "SoundLib\ISoundSystem.h"

namespace FMOD {
	class System;

	namespace Studio
	{
		class System;
	}
}

namespace TL_Sound
{
	class SoundSystem : public ISoundSystem
	{
	public:
		SoundSystem();
		virtual ~SoundSystem();

		static SoundSystem* Instance();

		virtual bool Initialize(uint32 maxChanelCnt) override;
		virtual void Release() override;

		//virtual bool LoadFile(const tstring& filePath, eFileType fileType) override;
		//virtual bool LoadFile(const std::string& filePath, eFileType fileType) override;
		virtual bool Tick(float deltaTime) override;
		virtual void Set3DSetting(float dopplerscale = 1, float distancefactor = 1, float rooloffscale = 1) override;

		virtual void SetListenerAttributes(ListenerAttributes& attributes) override;


		virtual SoundSource* CreateSoundSource() override;
		//virtual SoundListener* CreateSoundListener() override;
		virtual Sound* CreateSound(const tstring& filePath, eFileType fileType) override;
		virtual Sound* CreateSound(const std::string& filePath, eFileType fileType) override;
		virtual Effect* CreateEffect() override;
		virtual ReverbZones* CreateReverbZones() override;


		virtual void DeleteSoundSource(SoundSource*) override;
		//virtual void DeleteSoundListener(SoundListener*) override;
		virtual void DeleteSound(Sound*) override;
		virtual void DeleteEffect(Effect*) override;
		virtual void DeleteReverbZones(ReverbZones*) override;

		inline FMOD::Studio::System* GetStudioSystem() { return m_pStudioSystem; }
		inline FMOD::System* GetCoreSystem() { return m_pCoreSystem; }
	protected:
		FMOD::Studio::System* m_pStudioSystem;
		FMOD::System* m_pCoreSystem;
	};
}