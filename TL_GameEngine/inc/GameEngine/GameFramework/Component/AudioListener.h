#pragma once

#include "Common.h"

#include "Math\TL_Math.h"

#include "GameEngine/GameFramework/Component/ComponentBase.h"

namespace TL_GameEngine
{
	enum class eAudioVelocityUpdateMode
	{
		Auto = 0
		, Fixed = 1
		, Dynamic = 2
	};

	class GAME_ENGINE_API AudioListener : public ComponentBase
	{
	public:
		AudioListener(class GameObject* _gameObject, const tstring& _typeName = TEXT("AudioListener"));
		virtual ~AudioListener();

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
		// Static Function
		//////////////////////////////////////////////////////////////////////////



		//static void GetOutputData(std::vector<float> samples, int channel);
		//static void GetSpectrumData(std::vector<float> samples, int channel, );

	private:
		static bool m_bPause;
		static float m_fVolume;
		eAudioVelocityUpdateMode m_eAudioVelocityUpdateMode;

		TL_Math::Vector3 m_PrevPosiiton;
	};
}