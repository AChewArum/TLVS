#pragma once

#include "GameEngine/internal/dll.h"
#include "Common_min.h"

namespace TL_Application
{
    class Window;
    class ApplicationContext;
    class GameTime;
    class Input;
}

namespace TL_Graphics
{
    class RenderSystem;
}

namespace TL_UI
{
    class AUserInterface;
}

namespace TL_GameEngine
{
    class GameWorld;
    class PhysicsSystem;
    class AudioSystem;

    class GAME_ENGINE_API GameApplication
    {
    public:
        static GameApplication* Instance;

        GameApplication();

        virtual ~GameApplication();

        bool Start(TL_Application::ApplicationContext* _context);

        void End();

        bool Tick();

        void RenderTick();

        virtual void OnApplicationStart();

        virtual void OnApplicationTick();

        virtual void OnApplicationEnd();

    private:
        PhysicsSystem* m_PhysicsSystem;
        AudioSystem* m_AudioSystem;
        GameWorld* m_GameWorld;

        TL_Application::ApplicationContext* m_AppContext;

        bool m_bExitFlag;
        bool m_bAlreadyExit;

    public:
        PhysicsSystem* GetPhysicsSystem() const { return m_PhysicsSystem; }
        AudioSystem* GetAudioSystem() const { return m_AudioSystem; }
        GameWorld* GetGameWorld() const { return m_GameWorld; }

        TL_Application::ApplicationContext* GetAppContext() const { return m_AppContext; }
        TL_Application::Window* GetWindow() const;
        TL_Application::GameTime* GetGameTime() const;
        TL_Application::Input* GetInput() const;
        TL_Graphics::RenderSystem* GetRenderSystem() const;
    };
}
