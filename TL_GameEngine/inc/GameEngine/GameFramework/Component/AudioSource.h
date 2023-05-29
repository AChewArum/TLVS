#pragma once

#include "Common.h"
#include "GameEngine\GameFramework\Component\ComponentBase.h"
#include "GameEngine\Resource\Resource.h"
#include "SoundLib\SoundInfo.h"

namespace TL_GameEngine
{
	class AudioClip;

	class GAME_ENGINE_API AudioSource : public ComponentBase
	{
	public:
		AudioSource(class GameObject* _gameObject, const tstring& _typeName = TEXT("AudioSource"));
		virtual ~AudioSource();

		//////////////////////////////////////////////////////////////////////////
		// Inherited Component
		//////////////////////////////////////////////////////////////////////////

		virtual void PreTick();

		virtual void PostTick();

		virtual void TryTick();

		virtual void Tick();

		virtual void OnStart();

		virtual void OnChangeEnable(bool _newEnable);

		//////////////////////////////////////////////////////////////////////////
		// Member Function
		//////////////////////////////////////////////////////////////////////////

		/// @brief Plays the clip.
		/// @param delay Deprecated. Delay in number of samples, assuming a 44100Hz 
		/// sample rate (meaning that Play(44100) will delay the playing by exactly 1 sec).
		void Play(uint64 delay = 0);

		/// @brief Pauses playing the clip.
		void Pause();

		/// @brief Stops playing the clip.
		void Stop();

		/// @brief Unpause the paused playback of this AudioSource.
		/// This function is similar to calling Play() on a paused AudioSource, except 
		/// that it will not create a new playback voice if it is not currently paused.
		void UnPause();

		//////////////////////////////////////////////////////////////////////////
		// Getter & Setter
		//////////////////////////////////////////////////////////////////////////

		void SetMute(bool flag);
		void SetByPassEffects(bool flag);
		void SetByPassListernerEffect(bool flag);
		void SetBypassReverbZones(bool flag);
		void SetLoop(bool flag);
		inline void SetPlayOnAwake(bool flag);
		void SetPriority(uint32 value);
		void SetVolume(float value);
		void SetPitch(float value);
		void SetMaxDistance(float value);
		void SetMinDistance(float value);
		void SetMinMaxDistance(float min, float max);
		void SetClip(Resource<AudioClip>& clip);

		inline bool GetMute() { return m_bMute; }
		inline bool GetBypassEffects() { return m_bBypassEffects; }
		inline bool GetBypassListenerEffect() { return m_bBypassEffects; }
		inline bool GetBypassReverbZones() { return m_bBypassReverbZones; }
		inline bool GetPlayOnAwake() { return m_bPlayOnAwake; }
		inline bool GetLoop() { return m_bLoop; }
		inline bool GetVolume() { return m_fVolume; }
		inline bool GetPitch() { return m_fPitch; }
		inline float GetMaxDistance() { return m_fMaxDistance; }
		inline float GetMinDistance() { return m_fMinDistance; }

		float GetTime();
		bool GetIsPlaying();
		bool GetIsVirtual();

	private:
		Resource<AudioClip> m_pAudioClip;

		bool m_bIsDirty;

		bool m_bMute;
		bool m_bBypassEffects;
		bool m_bBypassListenerEffect;
		bool m_bBypassReverbZones;
		bool m_bPlayOnAwake;
		bool m_bLoop;

		uint32 m_iPriority;
		float m_fVolume;
		float m_fPitch;

		TL_Sound::eAudioRolloffMode m_eAudioRolloffMode;

		float m_fMaxDistance;
		float m_fMinDistance;

		TL_Sound::SoundSource* m_pSoundSource;
		//TL_Sound::Channel* m_pChannel;

		TL_Math::Vector3 m_PrevPosition;
	};
}
	