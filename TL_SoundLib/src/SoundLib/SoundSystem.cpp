#include "SoundLib\SoundPCH.h"
#include "SoundLib\SoundSystem.h"
#include "SoundLib\SoundSource.h"
#include "SoundLib\SoundListener.h"

#include "SoundLib\Metal\Sound.h"
#include "SoundLib\Metal\Effect.h"
#include "SoundLib\Metal\ReverbZones.h"


TL_Sound::SoundSystem* g_pInstance = nullptr;

namespace TL_Sound
{	
	SoundSystem::SoundSystem()
	: m_pCoreSystem(nullptr)
	, m_pStudioSystem(nullptr)
	{
		g_pInstance = this;
	}
	
	SoundSystem::~SoundSystem()
	{
		g_pInstance = nullptr;
	}

	SoundSystem* SoundSystem::Instance()
	{
		if (g_pInstance == nullptr)
		{
			g_pInstance = new SoundSystem;
		}

		return g_pInstance;
	}

	bool SoundSystem::Initialize(uint32 maxChanelCnt)
	{
		FMOD::Studio::System* _studioSystem = nullptr;
		FMOD::System* _coreSystem = nullptr;

		//AssertMessageBox(FMOD::Studio::System::create(&_studioSystem) == FMOD_OK, "FMOD System create failed");
		FMOD::Studio::System::create(&_studioSystem);
		_studioSystem->getCoreSystem(&_coreSystem);

		//AssertMessageBox(_studioSystem->initialize(maxChanelCnt, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) == FMOD_OK, "FMOD Studio init failed");
		_studioSystem->initialize(maxChanelCnt, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
		m_pStudioSystem = _studioSystem;
		m_pCoreSystem = _coreSystem;

		return true;
	}

	void SoundSystem::Release()
	{
		m_pStudioSystem->release();
	}

	bool SoundSystem::Tick(float deltaTime)
	{
		FMOD_RESULT result = m_pStudioSystem->update();
		
		AssertMessageBox(result == FMOD_OK, "Sound update failed");

		return true;
	}

	void SoundSystem::Set3DSetting(float dopplerscale /*= 1*/, float distancefactor /*= 1*/, float rooloffscale /*= 1*/)
	{
		m_pCoreSystem->set3DSettings(dopplerscale, distancefactor, rooloffscale);
	}

	void SoundSystem::SetListenerAttributes(ListenerAttributes& attributes)
	{
		FMOD_VECTOR _position = Vector3ToFMODVector(attributes._position);
		FMOD_VECTOR _velocity = Vector3ToFMODVector(attributes._velocity);
		FMOD_VECTOR _forward = Vector3ToFMODVector(attributes._forward);
		FMOD_VECTOR _up = Vector3ToFMODVector(attributes._up);

		m_pCoreSystem->set3DListenerAttributes(0, &_position, &_velocity, &_forward, &_up);
	}

	SoundSource* SoundSystem::CreateSoundSource()
	{
		return new SoundSource();
	}

	/*SoundListener* SoundSystem::CreateSoundListener()
	{
		return new SoundListener();
	}*/

	Sound* SoundSystem::CreateSound(const std::string& filePath, eFileType fileType)
	{
		return new Sound(filePath, fileType);
	}

	Sound* SoundSystem::CreateSound(const tstring& filePath, eFileType fileType)
	{
		std::string _filePath = StringHelper::ToString(filePath);

		Sound* _newSound = new Sound(_filePath, fileType);

		return _newSound;
	}

	Effect* SoundSystem::CreateEffect()
	{
		return new Effect();
	}

	TL_Sound::ReverbZones* SoundSystem::CreateReverbZones()
	{
		return new ReverbZones();
	}

	void SoundSystem::DeleteSoundSource(SoundSource* source)
	{
		SAFE_DELETE(source);
	}

	/*void SoundSystem::DeleteSoundListener(SoundListener* listener)
	{
		SAFE_DELETE(listener);
	}*/

	void SoundSystem::DeleteSound(Sound* sound)
	{
		SAFE_DELETE(sound);
	}

	void SoundSystem::DeleteEffect(Effect* effect)
	{
		SAFE_DELETE(effect);
	}

	void SoundSystem::DeleteReverbZones(ReverbZones* reverbZones)
	{
		SAFE_DELETE(reverbZones);
	}

}