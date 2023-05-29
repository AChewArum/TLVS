#include "SoundLib\SoundPCH.h"
#include "SoundLib\SoundSource.h"
#include "SoundLib\SoundSystem.h"

#include "SoundLib\Metal\Channel.h"
#include "SoundLib\Metal\Sound.h"

namespace TL_Sound
{

	SoundSource::SoundSource()
		: m_pChannel(nullptr)
		, m_pSound(nullptr)
	{
		m_pChannel = new Channel();
	}

	SoundSource::~SoundSource()
	{
		SAFE_DELETE(m_pChannel);
	}

	void SoundSource::SetClip(Sound* sound)
	{
		m_pSound = sound;
	}

	bool SoundSource::Play(uint64 delay)
	{
		if (m_pSound == nullptr)
		{
			return false;
		}

		if (SoundSystem::Instance()->GetCoreSystem()->playSound(m_pSound->GetSound(), nullptr, false, m_pChannel->GetChannelRef()) == FMOD_OK)
		{
			m_pChannel->SetIsVaild(true);
		}
		else
		{
			assert(false);
		}

		return true;
	}

	bool SoundSource::IsPlaying(bool* flag)
	{
		return m_pChannel->IsPlaying(flag);
	}

	bool SoundSource::IsVirtual(bool* flag)
	{
		return m_pChannel->IsVirtual(flag);
	}

	bool SoundSource::SetFrequency(float frequency)
	{
		return m_pChannel->SetFrequency(frequency);
	}

	bool SoundSource::SetPriority(uint32 priority)
	{
		return m_pChannel->SetPriority(priority);
	}

	uint32 SoundSource::GetPosition()
	{
		return m_pChannel->GetPosition();
	}

	bool SoundSource::Stop()
	{
		if (m_pSound == nullptr)
		{
			return false;
		}

		return m_pChannel->Stop();
	}

	bool SoundSource::SetPaused(bool flag)
	{
		return m_pChannel->SetPaused(flag);
	}

	bool SoundSource::SetLoop(bool flag)
	{
		return m_pChannel->SetLoop(flag);
	}

	bool SoundSource::SetPitch(float pitch)
	{
		return m_pChannel->SetPitch(pitch);
	}

	bool SoundSource::Set3DAttributes(TL_Math::Vector3& position, TL_Math::Vector3& velocity)
	{
		return m_pChannel->Set3DAttributes(position, velocity);
	}

	bool SoundSource::SetEffect(class Effect* effect)
	{
		return m_pChannel->SetEffect(effect);
	}

	bool SoundSource::RemoveEffect(class Effect* effect)
	{
		return m_pChannel->RemoveEffect(effect);
	}

	bool SoundSource::Set3DMinMaxDistance(float min, float max)
	{
		return m_pChannel->Set3DMinMaxDistance(min, max);
	}

	bool SoundSource::Set3DSpread(float angle)
	{
		return m_pChannel->Set3DSpread(angle);
	}

	bool SoundSource::SetMute(bool flag)
	{
		return m_pChannel->SetMute(flag);
	}

	bool SoundSource::SetVolume(float volume)
	{
		return m_pChannel->SetVolume(volume);
	}

	bool SoundSource::SetRolloffMode(eAudioRolloffMode mode)
	{
		return m_pChannel->SetRolloffMode(mode);
	}

	bool SoundSource::SetReverbProperties(bool flag)
	{
		return false;
	}
}