#include "SoundLib\SoundPCH.h"
#include "SoundLib\SoundSystem.h"
#include "SoundLib\Metal\Sound.h"

namespace TL_Sound
{
	Sound::Sound(const std::string& filePath, eFileType fileType)
		: m_pSound(nullptr)
	{
		auto _result = SoundSystem::Instance()->GetCoreSystem()->createSound(filePath.c_str(), FMOD_3D, 0, &m_pSound);
		
		AssertMessageBox(_result == FMOD_OK, "Create sound failed");
	}

	Sound::~Sound()
	{
		Release();
	}

	void Sound::Release()
	{
		SAFE_RELEASE(m_pSound);
	}

	float Sound::GetLength()
	{
		uint32 _ms;

		m_pSound->getLength(&_ms, FMOD_TIMEUNIT_MS);
		
		return static_cast<float>(_ms) / 1000.f;
	}

	FMOD::Sound* Sound::GetSound()
	{
		return m_pSound;
	}
}