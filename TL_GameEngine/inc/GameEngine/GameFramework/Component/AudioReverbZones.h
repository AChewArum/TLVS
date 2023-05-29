#pragma once

#include "Common.h"
#include "GameEngine\GameFramework\Component\ComponentBase.h"
#include "SoundLib\SoundInfo.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API AudioReverbZones : public ComponentBase
	{
	public:
		AudioReverbZones(class GameObject* _gameObject, const tstring& _typeName = TEXT("AudioReverbZones"));
		virtual ~AudioReverbZones();

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
		// Getter & Setter
		//////////////////////////////////////////////////////////////////////////

		inline void SetMinDistance(float value);
		inline void SetMaxDistance(float value);
		inline void SetReverbPreset(TL_Sound::eReverbPreset preset);

		inline void SetReverbProperties(TL_Sound::ReverbProperties& properties);
		inline void SetDecayTime(float value);
		inline void SetEarlyDelay(float value);
		inline void SetLateDelay(float value);
		inline void SetHFReference(float value);
		inline void SetHFDecayRatio(float value);
		inline void SetDiffusion(float value);
		inline void SetDensity(float value);
		inline void SetLowShelfFrequency(float value);
		inline void SetLowShelfGain(float value);
		inline void SetHighCut(float value);
		inline void SetEarlyLateMix(float value);
		inline void SetWetLevel(float value);

		inline float GetMinDistance()			{ return m_fMinDistance; }
		inline float GetMaxDistance()			{ return m_fMaxDistance; }
		inline float GetDecayTime()				{ return m_ReverbProperties._decayTime; }
		inline float GetEarlyDelay()			{ return m_ReverbProperties._earlyDelay; }
		inline float GetLateDelay()				{ return m_ReverbProperties._lateDelay; }
		inline float GetHFDecayRatio()			{ return m_ReverbProperties._hfDecayRatio; }
		inline float GetDiffusion()				{ return m_ReverbProperties._diffusion; }
		inline float GetDensity()				{ return m_ReverbProperties._density; }
		inline float GetLowShelfFrequencye()	{ return m_ReverbProperties._lowShelfFrequency; }
		inline float GetLowShelfGain()			{ return m_ReverbProperties._lowShelfGain; }
		inline float GetHighCut()				{ return m_ReverbProperties._highCut; }
		inline float GetEarlyLateMix()			{ return m_ReverbProperties._earlyLateMix; }
		inline float GetWetLevel()				{ return m_ReverbProperties._wetLevel; }

		TL_Sound::ReverbProperties& GetReverbProperties() { return m_ReverbProperties; }

	private:
		bool m_bIsDirty;

		float m_fMinDistance;
		float m_fMaxDistance;

		TL_Sound::ReverbProperties m_ReverbProperties;
		TL_Sound::eReverbPreset m_eReverbPreset;
		TL_Sound::ReverbZones* m_pReverbZones;

		TL_Math::Vector3 m_PrevPos;
	};
}