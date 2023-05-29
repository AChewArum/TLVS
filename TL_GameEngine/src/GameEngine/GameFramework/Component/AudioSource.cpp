#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\AudioSource.h"
#include "GameEngine\GameFramework\Component\Transform.h"
#include "GameEngine\GameFramework\Sound\AudioSystem.h"
#include "GameEngine\GameFramework\GameObject.h"

#include "GameEngine\GameFramework\Sound\AudioClip.h"

#include "SoundLib/SoundSource.h"

namespace TL_GameEngine
{
	AudioSource::AudioSource(GameObject* _gameObject, const tstring& _typeName)
		: ComponentBase(_gameObject, _typeName)
		, m_bIsDirty(false)
		, m_bLoop(true)
		, m_bMute(false)
		, m_bBypassEffects(false)
		, m_bBypassListenerEffect(false)
		, m_bBypassReverbZones(false)
		, m_bPlayOnAwake(true)
		, m_iPriority(128)
		, m_fVolume(1)
		, m_fPitch(1)
		, m_eAudioRolloffMode(TL_Sound::eAudioRolloffMode::Linear)
		, m_fMaxDistance(500)
		, m_fMinDistance(0)
		, m_pSoundSource(nullptr)
	{
	}

	AudioSource::~AudioSource()
	{
		AudioSystem::GetInstance()->DeleteSoundSource(m_pSoundSource);
	}

	void AudioSource::PreTick()
	{

	}

	void AudioSource::PostTick()
	{

	}

	void AudioSource::TryTick()
	{

	}

	void AudioSource::Tick()
	{
		if (m_bIsDirty || m_pAudioClip == nullptr)
		{
			return;
		}

		TL_Math::Vector3 _currPosition = m_GameObject->GetComponent<Transform>()->GetWorldPosition();

		TL_Math::Vector3 _currVelocity = _currPosition - m_PrevPosition;

		m_PrevPosition = _currPosition;

		if (_currVelocity != TL_Math::Vector3::Zero)
		{
			m_pSoundSource->Set3DAttributes(_currPosition, _currVelocity);
		}
	}

	void AudioSource::OnStart()
	{
		// Todo : Load Clip
		m_pSoundSource = AudioSystem::GetInstance()->CreateSoundSource();

		if (m_pAudioClip == nullptr)
		{
			return;
		}

		m_pSoundSource->SetClip(m_pAudioClip.GetPtr()->GetSound());

		m_bPlayOnAwake ? Play(0) : Stop();
	}

	void AudioSource::OnChangeEnable(bool _newEnable)
	{
		(_newEnable && m_bPlayOnAwake) ? m_pSoundSource->Play(0) : m_pSoundSource->Stop();
	}

	void AudioSource::Play(uint64 delay)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		if (m_pSoundSource->Play(delay) && m_bIsDirty)
		{
			m_pSoundSource->SetClip(m_pAudioClip->GetSound());
			m_pSoundSource->SetMute(m_bMute);
			m_pSoundSource->SetLoop(m_bLoop);
			m_pSoundSource->SetPriority(m_iPriority);
			m_pSoundSource->SetVolume(m_fVolume);
			m_pSoundSource->SetPitch(m_fPitch);
			m_pSoundSource->SetRolloffMode(m_eAudioRolloffMode);
			m_pSoundSource->Set3DMinMaxDistance(0, 10);
			m_bIsDirty = false;
		}
	}

	void AudioSource::Pause()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_pSoundSource->SetPaused(true);
	}

	void AudioSource::Stop()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_pSoundSource->Stop();
	}

	void AudioSource::UnPause()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_pSoundSource->SetPaused(false);
	}

	void AudioSource::SetMute(bool flag)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_bMute = flag;

		m_pSoundSource->SetMute(flag);
	}

	void AudioSource::SetByPassEffects(bool flag)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_bBypassEffects = flag;
	}

	void AudioSource::SetByPassListernerEffect(bool flag)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_bBypassListenerEffect = flag;
	}

	void AudioSource::SetBypassReverbZones(bool flag)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_bBypassReverbZones = flag;

		if (m_bIsDirty || !m_pSoundSource->SetReverbProperties(flag))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetLoop(bool flag)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_bLoop = flag;

		if (m_bIsDirty || !m_pSoundSource->SetLoop(flag))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetPlayOnAwake(bool flag)
	{
		m_bPlayOnAwake = flag;
	}

	void AudioSource::SetPriority(uint32 value)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_iPriority = value;

		if (m_bIsDirty || !m_pSoundSource->SetPriority(value))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetVolume(float value)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_fVolume = value;

		if (m_bIsDirty || !m_pSoundSource->SetVolume(value))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetPitch(float value)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");
		
		m_fPitch = value;

		if(m_bIsDirty || m_pSoundSource->SetPitch(value))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetMaxDistance(float value)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_fMaxDistance = value;

		if(m_bIsDirty || m_pSoundSource->Set3DMinMaxDistance(m_fMinDistance, m_fMaxDistance))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetMinDistance(float value)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_fMinDistance = value;

		if(m_bIsDirty || m_pSoundSource->Set3DMinMaxDistance(m_fMinDistance, m_fMaxDistance))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetMinMaxDistance(float min, float max)
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		m_fMinDistance = min;
		m_fMaxDistance = max;

		if(m_bIsDirty || m_pSoundSource->Set3DMinMaxDistance(min, max))
		{
			m_bIsDirty = true;
		}
	}

	void AudioSource::SetClip(Resource<AudioClip>& clip)
	{
		if (m_pAudioClip != nullptr && m_pSoundSource != nullptr)
		{
			m_pSoundSource->Stop();

			m_bIsDirty = true;
		}

		m_pAudioClip = clip;

		if (m_pSoundSource == nullptr)
		{
			m_bIsDirty = true;
			return;
		}

		m_pSoundSource->SetClip(clip->GetSound());
	}

	float AudioSource::GetTime()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		float _ms = static_cast<float>(m_pSoundSource->GetPosition()) / 1000.f;

		return _ms;
	}

	bool AudioSource::GetIsPlaying()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		bool _ret;

		if (m_bIsDirty || m_pSoundSource->IsPlaying(&_ret))
		{
			return false;
		}

		return _ret;
	}

	bool AudioSource::GetIsVirtual()
	{
		AssertMessageBox(m_pSoundSource != nullptr, "SoundSource is null");

		bool _ret;

		if (m_bIsDirty || m_pSoundSource->IsVirtual(&_ret))
		{
			return false;
		}

		return _ret;
	}

}