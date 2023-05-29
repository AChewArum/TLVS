#pragma once

#include "Common.h"
#include "SoundLib\FMODAPI.h"
#include "SoundLib\SoundInfo.h"

namespace TL_Math
{
	struct Vector3;
}

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC Channel
	{
	public:
		Channel();
		~Channel();

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

		FMOD::Channel* GetChannel() { return m_pChannel; }
		FMOD::Channel** GetChannelRef() { return &m_pChannel; }

		void SetIsVaild(bool flag) { m_bIsVaild = flag; }

	private:
		FMOD::Channel* m_pChannel;

		bool m_bIsVaild;
	};
}