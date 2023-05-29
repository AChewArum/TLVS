#pragma once

#include "Common.h"
#include "SoundLib\FMODAPI.h"
#include "SoundLib\SoundInfo.h"

namespace TL_Sound
{ 
	class SOUND_DLL_DECLSPEC Effect
	{
	public:
		Effect();
		virtual ~Effect();

		// Create DSP Effect
		virtual void initialize(FMOD::System* system, eEffectType effectType);

		virtual FMOD::DSP* GetEffect() { return m_pEffect; }

	private:
		FMOD::DSP* m_pEffect;
	};
}