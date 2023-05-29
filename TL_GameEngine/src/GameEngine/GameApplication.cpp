#include "GameEngine_pch.h"
#include "GameEngine/GameApplication.h"

#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/Gameframework/Physics/PhysicsSystem.h"
#include "GameEngine/Gameframework/Sound/AudioSystem.h"
#include "GameEngine/Resource/Resources.h"

#include "Application/ApplicationContext.h"
#include "Application/GameTime.h"

#include "Graphics/RenderSystem.h"
#include "Log/Log.h"

namespace TL_GameEngine
{
	using namespace TL_Common;

	GameApplication* GameApplication::Instance = nullptr;

	GameApplication::GameApplication()
	{
		assert(Instance == nullptr);
		Instance = this;
	}

	GameApplication::~GameApplication()
	{
		assert(Instance != nullptr);
		Instance = nullptr;

		if (m_bAlreadyExit == false)
			End();
	}

	bool GameApplication::Start(TL_Application::ApplicationContext* _context)
	{
		m_AppContext = _context;
		m_PhysicsSystem = nullptr;
		m_AudioSystem = nullptr;
		m_GameWorld = nullptr;
		m_bExitFlag = false;
		m_bAlreadyExit = false;

		SystemTimer _timer;

		if (_context->IsHeadless() == false)
		{
			// Windows 환경에서만 실행됩니다.
		}

		m_PhysicsSystem = &PhysicsSystem::Instance();
		m_AudioSystem = new AudioSystem();
		m_GameWorld = new GameWorld();
		new Resources(); // TODO. application context 내에서 resources 클래스를 가지고 있도록 수정해야 합니다. editor에서도 resources를 사용할 수 있어야 하기 때문입니다.
		m_bExitFlag = false;

		// PhysicsSystem을 초기화합니다.

		_timer.Lap();
		Log::Engine_Info("PhysicsSystem Start Initialize.");
		auto _physicsInfo = TL_Physics::PhysicsInfo();
		m_PhysicsSystem->Initalize(_physicsInfo);
		Log::Engine_Info("PhysicsSystem Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());

		m_AudioSystem->Initialize();
		Log::Engine_Info("AudioSystem Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());

		// GameWorld를 초기화합니다.

		_timer.Lap();
		Log::Engine_Info("GameWorld Start Initialize.");
		m_GameWorld->Initialize();
		Log::Engine_Info("GameWorld Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());


		// Application Start 콜백을 호출합니다.

		_timer.Lap();
		Log::Engine_Info("OnApplicationStart");
		OnApplicationStart();
		Log::Engine_Info("OnApplicationStart Finished. ({0} ms)", _timer.Lap().AsMillis());

		m_GameWorld->OnStart();

		return true;
	}

	void GameApplication::End()
	{
		m_bAlreadyExit = true;

		SystemTimer _timer;

		// GameWorld를 해제합니다.

		_timer.Lap();
		Log::Engine_Info("GameWorld Start Release.");
		m_GameWorld->Release();
		delete m_GameWorld;
		m_GameWorld = nullptr;
		Log::Engine_Info("GameWorld Finish Release. ({0} ms)", _timer.Lap().AsMillis());

		// Application End 콜백을 호출합니다.

		_timer.Lap();
		Log::Engine_Info("OnApplicationEnd");
		OnApplicationEnd();
		Log::Engine_Info("OnApplicationEnd Finished. ({0} ms)", _timer.Lap().AsMillis());

		delete Resources::GetInstance(); // TODO

		// AudioSystem을 해제합니다.

		_timer.Lap();
		Log::Engine_Info("AudioSystem Start Release.");
		m_AudioSystem->Release();
		delete m_AudioSystem;
		m_AudioSystem = nullptr;
		Log::Engine_Info("PhysicsSystem Finish Release. ({0} ms)", _timer.Lap().AsMillis());

		// PhysicsSystem을 해제합니다.

		_timer.Lap();
		Log::Engine_Info("PhysicsSystem Start Release.");
		m_PhysicsSystem->Release();
		m_PhysicsSystem = nullptr;
		Log::Engine_Info("PhysicsSystem Finish Release. ({0} ms)", _timer.Lap().AsMillis());
	}

	bool GameApplication::Tick()
	{
		SystemTimer _timer;
		_timer.Lap();
		{
			const bool _isNotEditorAndNotHeadless = m_AppContext->IsEditor() == false && m_AppContext->IsHeadless() == false;

			SystemTimer _innerTimer;

			if (_isNotEditorAndNotHeadless)
			{
				/* PreTick */
				m_AppContext->PreTick();
			}

			static float _timerfps = 0;
			_timerfps += m_AppContext->GetGameTime()->GetDeltaTime();

			static float _fps = 0;
			_fps += 1;
			if (_timerfps >= 1)
			{
				Log::Engine_Info("fps " + std::to_string(_fps));
				_fps = 0;
				_timerfps = 0;
			}


			/* Tick */

			_innerTimer.Lap();
			Log::Engine_Trace("AudioSystem Start Tick.");
			m_AudioSystem->Tick(m_AppContext->GetGameTime()->GetDeltaTime());
			Log::Engine_Trace("AudioSystem Finish Tick. ({0} ms)", _timer.Lap().AsMillis());


			_innerTimer.Lap();
			Log::Engine_Trace("PhysicsSystem Start Tick.");
			m_PhysicsSystem->Tick(m_AppContext->GetGameTime()->GetDeltaTime());
			Log::Engine_Trace("PhysicsSystem Finish Tick. ({0} ms)", _timer.Lap().AsMillis());

			// GameWorld Tick
			_innerTimer.Lap();
			Log::Engine_Trace("GameWorld Start Tick.");
			m_GameWorld->Tick();
			Log::Engine_Trace("GameWorld Finish Tick. ({0} ms)", _timer.Lap().AsMillis());


			// OnApplicationTick 콜백을 호출합니다.
			_innerTimer.Lap();
			Log::Engine_Trace("OnApplicationTick Start.");
			OnApplicationTick();
			Log::Engine_Trace("OnApplicationTick Finished. ({0} ms)", _timer.Lap().AsMillis());
		}
		const double _elapsedTickTimeMillis = _timer.Lap().AsMillis();
		Log::Engine_Trace("Tick Finished. ({0} ms, {1} fps)", _elapsedTickTimeMillis, 1'000 / _elapsedTickTimeMillis);

		return m_bExitFlag;
	}

	void GameApplication::RenderTick()
	{
		SystemTimer _timer;

		const bool _isNotEditorAndNotHeadless = m_AppContext->IsEditor() == false && m_AppContext->IsHeadless() == false;

		if (_isNotEditorAndNotHeadless)
		{
			_timer.Lap();
			Log::Engine_Trace("RenderSystem Start Render.");
			m_AppContext->PreRenderObjects();
		}

		m_GameWorld->Render();

		if (_isNotEditorAndNotHeadless)
		{
			m_AppContext->PostRenderObjects();
			m_AppContext->Present();
			Log::Engine_Trace("RenderSystem Finish Render. ({0} ms)", _timer.Lap().AsMillis());
		}
	}

	void GameApplication::OnApplicationStart() {}

	void GameApplication::OnApplicationTick() {}

	void GameApplication::OnApplicationEnd() { }

	TL_Application::Window* GameApplication::GetWindow() const { return m_AppContext->GetWindow(); }
	TL_Application::GameTime* GameApplication::GetGameTime() const { return m_AppContext->GetGameTime(); }
	TL_Application::Input* GameApplication::GetInput() const { return m_AppContext->GetInput(); }
	TL_Graphics::RenderSystem* GameApplication::GetRenderSystem() const { return m_AppContext->GetRenderSystem(); }
}
