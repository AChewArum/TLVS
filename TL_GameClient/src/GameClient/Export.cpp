#include "GameClient/TestScenes/MainTestApp.h"
#include "GameClient/TestScenes/MoveTestApp.h"
#include "GameClient/TestApplication/TestFirstStageGameApplication.h"

#include "GameClient/TestApplication/TestFbxGraphicsApplication.h"
#include "GameClient/TestApplication/TestGameApplication.h"
#include "GameClient/TestApplication/TestGraphicsApplication.h"
#include "GameClient/TestApplication/TestPhysicsApplication.h"
#include "TestScenes/StageTestApp.h"
#include "MainScene/Stage29.h"
#include "MainScene/Stage33.h"

#if defined(GAME_CLIENT_BUILD_DLL)
/* We are building Hashing as a Win32 DLL */
#define GAME_CLIENT_API __declspec(dllexport)
#else
/* We are calling Hashing as a Win32 DLL */
#define GAME_CLIENT_API __declspec(dllimport)
#endif

extern "C"
{
GAME_CLIENT_API TL_GameEngine::GameApplication* CreateApplication(const tstring& _appType)
{
    if (_appType == TEXT("TestGame"))
        return new TestGameApplication();


    else if (_appType == TEXT("TestGraphics"))
        return new TestGraphicsApplication();

    else if (_appType == TEXT("TestPhysics"))
        return new TestPhysicsApplication();

    else if (_appType == TEXT("TestFbxGraphics"))
        return new TestFbxGraphicsApplication();

    else if (_appType == TEXT("TestFirstStageGame"))
        return new TestFirstStageGameApplication();

    else if (_appType == TEXT("MainTestApp"))
        return new MainTestApp();

    else if (_appType == TEXT("MoveTestApp"))
        return new MoveTestApp();

    else if (_appType == TEXT("StageTestApp"))
        return new StageTestApp();

    else if (_appType == TEXT("Stage29"))
        return new Stage29();
    
    else if (_appType == TEXT("Stage33"))
        return new Stage33();

    else
        return new TL_GameEngine::GameApplication();
}

GAME_CLIENT_API void ReleaseApplication(TL_GameEngine::GameApplication* _app)
{
    delete _app;
}
}
