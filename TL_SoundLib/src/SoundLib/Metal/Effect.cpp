#include "SoundLib\SoundPCH.h"
#include "SoundLib\Metal\Effect.h"

namespace TL_Sound
{
	Effect::Effect()
		: m_pEffect(nullptr)
	{
	}

	Effect::~Effect()
	{
	}

	void Effect::initialize(FMOD::System* system, eEffectType effectType)
	{
		FMOD_DSP_DESCRIPTION _desc;

		AssertMessageBox(system->createDSPByType((FMOD_DSP_TYPE)effectType, &m_pEffect) != FMOD_OK, "Effect Create failed");
	}
}