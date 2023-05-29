#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Component\AudioReverbZones.h"
#include "GameEngine\GameFramework\Component\Transform.h"
#include "GameEngine\GameFramework\Sound\AudioSystem.h"
#include "GameEngine\GameFramework\GameObject.h"

#include "SoundLib\Metal\ReverbZones.h"

namespace TL_GameEngine
{

	AudioReverbZones::AudioReverbZones(class GameObject* _gameObject, const tstring& _typeName /*= TEXT("AudioReverbZones")*/)
		: ComponentBase(_gameObject, _typeName)
		, m_fMinDistance(10)
		, m_fMaxDistance(15)
		, m_eReverbPreset(TL_Sound::eReverbPreset::PRESET_GENERIC)
	{

	}

	AudioReverbZones::~AudioReverbZones()
	{
		AudioSystem::GetInstance()->DeleteReverbZones(m_pReverbZones);

		m_pReverbZones = nullptr;
	}

	void AudioReverbZones::PreTick()
	{

	}

	void AudioReverbZones::PostTick()
	{

	}

	void AudioReverbZones::TryTick()
	{

	}

	void AudioReverbZones::Tick()
	{
		TL_Math::Vector3 _currPos = m_GameObject->GetComponent<Transform>()->GetWorldPosition();

		if (m_bIsDirty || _currPos == m_PrevPos)
		{
			TL_Sound::ReverbAttributes _attributes{
				_currPos
				, m_fMinDistance
				, m_fMaxDistance
			};

			m_pReverbZones->Set3DAttributes(_attributes);
			m_pReverbZones->SetProperties(m_ReverbProperties);
			m_bIsDirty = true;
		}

		m_PrevPos = _currPos;
	}

	void AudioReverbZones::OnStart()
	{
		m_pReverbZones = AudioSystem::GetInstance()->CreateReverbZones();
	}

	void AudioReverbZones::OnChangeEnable(bool _newEnable)
	{
		m_pReverbZones->SetActive(_newEnable);
	}

	void AudioReverbZones::SetMinDistance(float value)
	{
		m_bIsDirty = true;

		m_fMinDistance = value;
	}

	void AudioReverbZones::SetMaxDistance(float value)
	{
		m_bIsDirty = true;

		m_fMaxDistance = value;
	}

	void AudioReverbZones::SetReverbPreset(TL_Sound::eReverbPreset preset)
	{
		m_bIsDirty = true;

		m_eReverbPreset = preset;

		switch (m_eReverbPreset)
		{
		case TL_Sound::eReverbPreset::PRESET_OFF:
			m_ReverbProperties = {  1000,    7,  11, 5000, 100, 100, 100, 250, 0,    20,  96, -80.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_GENERIC:
			m_ReverbProperties = {  1500,    7,  11, 5000,  83, 100, 100, 250, 0, 14500,  96,  -8.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_PADDEDCELL:
			m_ReverbProperties = {   170,    1,   2, 5000,  10, 100, 100, 250, 0,   160,  84,  -7.8f };
			break;
		case TL_Sound::eReverbPreset::PRESET_ROOM:
			m_ReverbProperties = {   400,    2,   3, 5000,  83, 100, 100, 250, 0,  6050,  88,  -9.4f };
			break;
		case TL_Sound::eReverbPreset::PRESET_BATHROOM:
			m_ReverbProperties = {  1500,    7,  11, 5000,  54, 100,  60, 250, 0,  2900,  83,   0.5f };
			break;
		case TL_Sound::eReverbPreset::PRESET_LIVINGROOM:
			m_ReverbProperties = {   500,    3,   4, 5000,  10, 100, 100, 250, 0,   160,  58, -19.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_STONEROOM:
			m_ReverbProperties = {  2300,   12,  17, 5000,  64, 100, 100, 250, 0,  7800,  71,  -8.5f };
			break;
		case TL_Sound::eReverbPreset::PRESET_AUDITORIUM:
			m_ReverbProperties = {  4300,   20,  30, 5000,  59, 100, 100, 250, 0,  5850,  64, -11.7f };
			break;
		case TL_Sound::eReverbPreset::PRESET_CONCERTHALL:
			m_ReverbProperties = {  3900,   20,  29, 5000,  70, 100, 100, 250, 0,  5650,  80,  -9.8f };
			break;
		case TL_Sound::eReverbPreset::PRESET_CAVE:
			m_ReverbProperties = {  2900,   15,  22, 5000, 100, 100, 100, 250, 0, 20000,  59, -11.3f };
			break;
		case TL_Sound::eReverbPreset::PRESET_ARENA:
			m_ReverbProperties = {  7200,   20,  30, 5000,  33, 100, 100, 250, 0,  4500,  80,  -9.6f };
			break;
		case TL_Sound::eReverbPreset::PRESET_HANGAR:
			m_ReverbProperties = { 10000,   20,  30, 5000,  23, 100, 100, 250, 0,  3400,  72,  -7.4f };
			break;
		case TL_Sound::eReverbPreset::PRESET_CARPETTEDHALLWAY:
			m_ReverbProperties = {   300,    2,  30, 5000,  10, 100, 100, 250, 0,   500,  56, -24.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_HALLWAY:
			m_ReverbProperties = {  1500,    7,  11, 5000,  59, 100, 100, 250, 0,  7800,  87,  -5.5f };
			break;
		case TL_Sound::eReverbPreset::PRESET_STONECORRIDOR:
			m_ReverbProperties = {   270,   13,  20, 5000,  79, 100, 100, 250, 0,  9000,  86,  -6.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_ALLEY:
			m_ReverbProperties = {  1500,    7,  11, 5000,  86, 100, 100, 250, 0,  8300,  80,  -9.8f };
			break;
		case TL_Sound::eReverbPreset::PRESET_FOREST:
			m_ReverbProperties = {  1500,  162,  88, 5000,  54,  79, 100, 250, 0,   760,  94, -12.3f };
			break;
		case TL_Sound::eReverbPreset::PRESET_CITY:
			m_ReverbProperties = {  1500,    7,  11, 5000,  67,  50, 100, 250, 0,  4050,  66, -26.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_MOUNTAINS:
			m_ReverbProperties = {  1500,  300, 100, 5000,  21,  27, 100, 250, 0,  1220,  82, -24.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_QUARRY:
			m_ReverbProperties = {  1500,   61,  25, 5000,  83, 100, 100, 250, 0,  3400, 100,  -5.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_PLAIN:
			m_ReverbProperties = {  1500,  179, 100, 5000,  50,  21, 100, 250, 0,  1670,  65, -28.0f };
			break;
		case TL_Sound::eReverbPreset::PRESET_PARKINGLOT:
			m_ReverbProperties = {  1700,    8,  12, 5000, 100, 100, 100, 250, 0, 20000,  56, -19.5f };
			break;
		case TL_Sound::eReverbPreset::PRESET_SEWERPIPE:
			m_ReverbProperties = {  2800,   14,  21, 5000,  14,  80,  60, 250, 0,  3400,  66,   1.2f };
			break;
		case TL_Sound::eReverbPreset::PRESET_UNDERWATER:
			m_ReverbProperties = {  1500,    7,  11, 5000,  10, 100, 100, 250, 0,   500,  92,   7.0f };
			break;
		default:
			assert(false);
			break;
		}

		m_bIsDirty = true;
	}

	void AudioReverbZones::SetReverbProperties(TL_Sound::ReverbProperties& properties)
	{
		m_pReverbZones->SetProperties(properties);
	}

	void AudioReverbZones::SetDecayTime(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._decayTime = std::clamp(value, 0.f, 20000.f);
	}

	void AudioReverbZones::SetEarlyDelay(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._earlyDelay = std::clamp(value, 0.f, 300.f);
	}

	void AudioReverbZones::SetLateDelay(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._lateDelay = std::clamp(value, 0.f, 100.f);
	}

	void AudioReverbZones::SetHFReference(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._hfReference = std::clamp(value, 20.f, 20000.f);
	}

	void AudioReverbZones::SetHFDecayRatio(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._hfDecayRatio = std::clamp(value, 10.f, 100.f);
	}

	void AudioReverbZones::SetDiffusion(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._diffusion = std::clamp(value, 10.f, 100.f);
	}

	void AudioReverbZones::SetDensity(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._density = std::clamp(value, 0.f, 100.f);
	}

	void AudioReverbZones::SetLowShelfFrequency(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._lowShelfFrequency = std::clamp(value, 20.f, 1000.f);
	}

	void AudioReverbZones::SetLowShelfGain(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._lowShelfGain = std::clamp(value, -36.f, 12.f);
	}

	void AudioReverbZones::SetHighCut(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._highCut = std::clamp(value, 0.f, 20000.f);
	}

	void AudioReverbZones::SetEarlyLateMix(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._earlyLateMix = std::clamp(value, 0.f, 100.f);
	}

	void AudioReverbZones::SetWetLevel(float value)
	{
		m_bIsDirty = true;

		m_ReverbProperties._wetLevel = std::clamp(value, -80.f, 20.f);
	}

}