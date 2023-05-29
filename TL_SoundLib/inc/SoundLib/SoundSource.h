#pragma once

#include "Common.h"
#include "Math\TL_Math.h"
#include "SoundLib\SoundInfo.h"

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC SoundSource
	{
	public:
		SoundSource();
		~SoundSource();

		void SetClip(Sound* sound);

		bool Play(uint64 delay);

		//////////////////////////////////////////////////////////////////////////
		// Channel Control
		//////////////////////////////////////////////////////////////////////////

		bool IsPlaying(bool* flag);

		bool IsVirtual(bool* flag);

		bool SetFrequency(float frequency);

		bool SetPriority(uint32 priority);

		uint32 GetPosition();

		bool Stop();

		bool SetPaused(bool flag);

		bool SetLoop(bool flag);

		bool SetPitch(float pitch);

		bool Set3DAttributes(TL_Math::Vector3& position, TL_Math::Vector3& velocity);

		bool SetEffect(class Effect* effect);

		bool RemoveEffect(class Effect* effect);

		bool Set3DMinMaxDistance(float min, float max);

		bool Set3DSpread(float angle);

		bool SetMute(bool flag);

		bool SetVolume(float volume);

		bool SetRolloffMode(eAudioRolloffMode mode);

		bool SetReverbProperties(bool flag);


	private:
		Sound* m_pSound;
		Channel* m_pChannel;
	};
};