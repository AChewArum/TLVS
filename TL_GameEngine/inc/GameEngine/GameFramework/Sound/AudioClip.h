#pragma once

#include "Common.h"

#include "GameEngine\internal\dll.h"

#include "SoundLib\SoundInfo.h"

namespace TL_GameEngine
{
	enum class eAudioDataLoadState
	{
		UNLOAD = 0
		, LOADING = 1
		, LOADED = 2
		, FAILED = 3
	};

	enum class eAudioClipLoadType
	{
		DECOMPRESSSONLOAD = 0
		, COMPRESSEDINMEMORY = 1
		, STREAMING = 2
	};

	class GAME_ENGINE_API AudioClip
	{
		friend class Resources;

	public:
		AudioClip();
		~AudioClip();

		bool GetData();

		void SetSound(TL_Sound::Sound* sound) { m_pSound = sound; }
		TL_Sound::Sound* GetSound() const { return m_pSound; }

		static AudioClip* Create(tstring& path, TL_Sound::eFileType type = TL_Sound::eFileType::WAV);

	private:
		/// @brief 	Returns true if this audio clip is ambisonic.
		bool m_bAmbisonic;

		/// @brief 	The number of channels in the audio clip.
		uint32 m_iChannaels;

		/// @brief The sample frequency of the clip in Hertz.
		uint32 m_iFrequency;

		/// @brief The length of the audio clip in seconds.
		float m_fLength;

		/// @brief Corresponding to the "Load In Background" flag in the inspector,
		/// when this flag is set, the loading will happen delayed without blocking
		/// the main thread.
		bool m_bLoadInBackground;

		/// @brief 	Returns the current load state of the audio data associated with an AudioClip.
		eAudioDataLoadState m_eAudioDataLoadState;

		/// @brief The load type of the clip 
		eAudioClipLoadType m_eAudioClipLoadType;

		/// @brief Preloads audio data of the clip when the clip asset is loaded.
		/// When this flag is off, scripts have to call AudioClip.LoadAudioData()
		/// to load the data before the clip can be played. Properties like length
		bool m_bPreLoadAudioData;

		/// @brief The length of the audio clip in samples.
		uint32 m_iSample;

		TL_Sound::Sound* m_pSound;
	};
}