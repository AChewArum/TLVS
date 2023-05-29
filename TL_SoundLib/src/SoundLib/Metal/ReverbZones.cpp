#include "SoundLib\SoundPCH.h"
#include "SoundLib\Metal\ReverbZones.h"
#include "SoundLib\SoundSystem.h"

namespace TL_Sound
{

	ReverbZones::ReverbZones()
		: m_pReverb3D(nullptr)
	{
		auto _result = SoundSystem::Instance()->GetCoreSystem()->createReverb3D(&m_pReverb3D);

		AssertMessageBox(_result == FMOD_OK, "Create Reverb3D failed");
	}

	ReverbZones::~ReverbZones()
	{
		SAFE_RELEASE(m_pReverb3D);
	}

	void ReverbZones::Set3DAttributes(ReverbAttributes& attributes)
	{
		FMOD_VECTOR _position = Vector3ToFMODVector(attributes._position);

		if (m_pReverb3D->set3DAttributes(&_position, attributes._minDistance, attributes._maxDistance) != FMOD_OK)
		{
			assert(false);
		}
	}

	void ReverbZones::SetPropertiesPreset(eReverbPreset preset)
	{
		FMOD_REVERB_PROPERTIES _properties;

		switch (preset)
		{
		case TL_Sound::eReverbPreset::PRESET_OFF:
			_properties = FMOD_PRESET_OFF;
			break;
		case TL_Sound::eReverbPreset::PRESET_GENERIC:
			_properties = FMOD_PRESET_GENERIC;
			break;
		case TL_Sound::eReverbPreset::PRESET_PADDEDCELL:
			_properties = FMOD_PRESET_PADDEDCELL;
			break;
		case TL_Sound::eReverbPreset::PRESET_ROOM:
			_properties = FMOD_PRESET_ROOM;
			break;
		case TL_Sound::eReverbPreset::PRESET_BATHROOM:
			_properties = FMOD_PRESET_BATHROOM;
			break;
		case TL_Sound::eReverbPreset::PRESET_LIVINGROOM:
			_properties = FMOD_PRESET_LIVINGROOM;
			break;
		case TL_Sound::eReverbPreset::PRESET_STONEROOM:
			_properties = FMOD_PRESET_STONEROOM;
			break;
		case TL_Sound::eReverbPreset::PRESET_AUDITORIUM:
			_properties = FMOD_PRESET_AUDITORIUM;
			break;
		case TL_Sound::eReverbPreset::PRESET_CONCERTHALL:
			_properties = FMOD_PRESET_CONCERTHALL;
			break;
		case TL_Sound::eReverbPreset::PRESET_CAVE:
			_properties = FMOD_PRESET_CAVE;
			break;
		case TL_Sound::eReverbPreset::PRESET_ARENA:
			_properties = FMOD_PRESET_ARENA;
			break;
		case TL_Sound::eReverbPreset::PRESET_HANGAR:
			_properties = FMOD_PRESET_HANGAR;
			break;
		case TL_Sound::eReverbPreset::PRESET_CARPETTEDHALLWAY:
			_properties = FMOD_PRESET_CARPETTEDHALLWAY;
			break;
		case TL_Sound::eReverbPreset::PRESET_HALLWAY:
			_properties = FMOD_PRESET_HALLWAY;
			break;
		case TL_Sound::eReverbPreset::PRESET_STONECORRIDOR:
			_properties = FMOD_PRESET_STONECORRIDOR;
			break;
		case TL_Sound::eReverbPreset::PRESET_ALLEY:
			_properties = FMOD_PRESET_ALLEY;
			break;
		case TL_Sound::eReverbPreset::PRESET_FOREST:
			_properties = FMOD_PRESET_FOREST;
			break;
		case TL_Sound::eReverbPreset::PRESET_CITY:
			_properties = FMOD_PRESET_CITY;
			break;
		case TL_Sound::eReverbPreset::PRESET_MOUNTAINS:
			_properties = FMOD_PRESET_MOUNTAINS;
			break;
		case TL_Sound::eReverbPreset::PRESET_QUARRY:
			_properties = FMOD_PRESET_QUARRY;
			break;
		case TL_Sound::eReverbPreset::PRESET_PLAIN:
			_properties = FMOD_PRESET_PLAIN;
			break;
		case TL_Sound::eReverbPreset::PRESET_PARKINGLOT:
			_properties = FMOD_PRESET_PARKINGLOT;
			break;
		case TL_Sound::eReverbPreset::PRESET_SEWERPIPE:
			_properties = FMOD_PRESET_SEWERPIPE;
			break;
		case TL_Sound::eReverbPreset::PRESET_UNDERWATER:
			_properties = FMOD_PRESET_UNDERWATER;
			break;
		default:
			assert(false);
			break;
		}

		if (m_pReverb3D->setProperties(&_properties) != FMOD_OK)
		{
			assert(false);
		}
	}

	void ReverbZones::SetProperties(ReverbProperties& properties)
	{
		auto* _properties = (FMOD_REVERB_PROPERTIES*)&properties;

		if (m_pReverb3D->setProperties(_properties) != FMOD_OK)
		{
			assert(false);
		}
	}

	void ReverbZones::SetActive(bool flag)
	{
		if (m_pReverb3D->setActive(flag) != FMOD_OK)
		{
			assert(false);
		}
	}
}