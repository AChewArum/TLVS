#pragma once

#include "Common.h"
#include "SoundLib\SoundInfo.h"

namespace FMOD
{
	class Reverb3D;
}

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC ReverbZones
	{
	public:
		ReverbZones();
		~ReverbZones();

		void Set3DAttributes(ReverbAttributes& attributes);

		void SetPropertiesPreset(eReverbPreset preset);
		void SetProperties(ReverbProperties& properties);

		void SetActive(bool flag);

	private:
		FMOD::Reverb3D* m_pReverb3D;

	};
}