#include "SoundLib\SoundPCH.h"
#include "SoundLib\Metal\Channel.h"

#include "SoundLib\Metal\Effect.h"

#define CheckChannel(ret) if(m_pChannel == nullptr) { return ret; }

namespace TL_Sound
{
	Channel::Channel()
		: m_bIsVaild(true)
		, m_pChannel(nullptr)
	{

	}

	Channel::~Channel()
	{

	}

	bool Channel::IsPlaying(bool* flag)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->isPlaying(flag) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::IsVirtual(bool* flag)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->isVirtual(flag) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetFrequency(float frequency)
	{
		CheckChannel(m_bIsVaild);

		if(m_pChannel->setFrequency(frequency) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetPriority(uint32 priority)
	{
		CheckChannel(m_bIsVaild);

		if(m_pChannel->setPriority(priority) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	uint32 Channel::GetPosition()
	{
		uint32 _ms = 0;

		CheckChannel(_ms);

		if(m_pChannel->getPosition(&_ms, FMOD_TIMEUNIT_MS) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return _ms;
	}

	bool Channel::Stop()
	{
		CheckChannel(m_bIsVaild);

		m_bIsVaild = true;

		if (m_pChannel->stop() != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetPaused(bool flag)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->setPaused(flag) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetLoop(bool flag)
	{
		CheckChannel(m_bIsVaild);

		FMOD_MODE mode = flag ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

		if (m_pChannel->setMode(mode) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetPitch(float pitch)
	{
		CheckChannel(m_bIsVaild);

		if(m_pChannel->setPitch(pitch) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::Set3DAttributes(TL_Math::Vector3& position, TL_Math::Vector3& velocity)
	{
		CheckChannel(m_bIsVaild);

		FMOD_VECTOR _position = Vector3ToFMODVector(position);
		FMOD_VECTOR _velocity = Vector3ToFMODVector(velocity);

		if (m_pChannel->set3DAttributes(&_position, &_velocity) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetEffect(Effect* effect)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->addDSP(0, effect->GetEffect()) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::RemoveEffect(Effect* effect)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->removeDSP(effect->GetEffect()) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::Set3DMinMaxDistance(float min, float max)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->set3DMinMaxDistance(min, max) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::Set3DSpread(float angle)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->set3DSpread(angle) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetMute(bool flag)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->setMute(flag) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetVolume(float volume)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->setVolume(volume) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

	bool Channel::SetRolloffMode(eAudioRolloffMode mode)
	{
		CheckChannel(m_bIsVaild);

		if (m_pChannel->setMode((FMOD_MODE)mode) != FMOD_OK)
		{
			m_pChannel = nullptr;
			m_bIsVaild = false;
		}

		return m_bIsVaild;
	}

}