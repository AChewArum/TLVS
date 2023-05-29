#include <Windows.h>
#include <cxxopts.hpp>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <shellapi.h>
#include <cstdio>

#include "Common.h"
#include "Application/ApplicationContext.h"
#include "GameEngine/GameApplication.h"

#if defined(DEBUG) | defined(_DEBUG)

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)

#if defined(DEBUG_ENABLE_VLD)

#include <vld.h>

#if defined(_WIN64)
#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win64\\vld.lib")
#elif
#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win32\\vld.lib")
#endif

#endif

#endif

using namespace TL_Common;

using PROC_CREATE_APPLICATION = TL_GameEngine::GameApplication* (*)(const tstring&);
using PROC_RELEASE_APPLICATION = void (*)(TL_GameEngine::GameApplication*);

void RedirectIOToConsole()
{
    // maximum mumber of lines the output console should have
    static const WORD MAX_CONSOLE_LINES = 500;

    int hConHandle;
    long lStdHandle;

    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    FILE* fp;

    // allocate a console for this app
    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

    coninfo.dwSize.Y = MAX_CONSOLE_LINES;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDOUT to the console

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);

    // redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen(hConHandle, "r");

    *stdin = *fp;
    setvbuf(stdin, NULL, _IONBF, 0);

    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stderr = *fp;
    setvbuf(stderr, NULL, _IONBF, 0);

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
    // point to console as well
    std::ios::sync_with_stdio();
}

int APIENTRY _tWinMain(_In_ HINSTANCE _hInstance,
                       _In_opt_ HINSTANCE _hPrevInstance,
                       _In_ LPTSTR _lpCmdLine,
                       _In_ int _nCmdShow
)
{
    UNREFERENCED_PARAMETER(_hPrevInstance);
    UNREFERENCED_PARAMETER(_lpCmdLine);

    #ifdef _DEBUG
    // 프로그램이 종료될 때 자동으로 _CrtDumpMemoryLeaks()를 호출합니다.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CrtSetBreakAlloc(929);
    #endif // _DEBUG

    // standard output을 보여주는 콘솔 창을 표시합니다.

    RedirectIOToConsole();

    // 라이브러리와 라이브러리 내 함수 주소를 불러옵니다.

    HMODULE _hDLL = LoadLibrary(TEXT("./GameClient.dll"));
    assert(_hDLL != nullptr);

    auto _proc_CreateApplication = (PROC_CREATE_APPLICATION)GetProcAddress(_hDLL, "CreateApplication");
    auto _proc_ReleaseApplication = (PROC_RELEASE_APPLICATION)GetProcAddress(_hDLL, "ReleaseApplication");
    assert(_proc_CreateApplication != nullptr);
    assert(_proc_ReleaseApplication != nullptr);

    auto _appContext = TL_Application::ApplicationContext::Create({
        false, false, _nCmdShow, _hInstance, TEXT("ThanksLightWindow"), TEXT("Thanks Light"), 1280, 720
    });

    if (_appContext.get() == nullptr)
        return -1;

    // TODO: GameEngine 내 최상위 클래스의 이름을 Application에서 GameApplication 등의 이름으로 변경해야 합니다.
    auto _app = _proc_CreateApplication(StringHelper::ToTString(_appContext->GetCommandLineArgs().find("app")->second));
    {
        if (_app->Start(_appContext.get()) == false)
        {
            return -1;
        }

        while (true)
        {
            MSG _msg;

            if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&_msg);
                DispatchMessage(&_msg);

                if (_msg.message == WM_QUIT)
                    break;
            }
            else
            {
                if (_app->Tick() == true)
                    break;

                _app->RenderTick();
            }
        }

        _app->End();
    }
    _proc_ReleaseApplication(_app);

    FreeLibrary(_hDLL);

    return 0;
}
