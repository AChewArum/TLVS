#pragma once

#include "Common.h"

#include "SoundLib\SoundInfo.h"

namespace TL_Sound
{
	class SOUND_DLL_DECLSPEC SoundListener
	{
	public:
		SoundListener() = default;
		~SoundListener() = default;

		void Tick();

		void Set3DListenerAttributes(ListenerAttributes& attributes);
	};
}