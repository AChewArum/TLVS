#include "GameEditor_pch.h"

#include <fcntl.h>

#include "GameEditor/EditorApplication.h"

using namespace TL_Common;

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
    // _CrtSetBreakAlloc(274165);
#endif // _DEBUG

    RedirectIOToConsole();

    auto _appContext = TL_Application::ApplicationContext::Create({
        true, false, _nCmdShow, _hInstance, TEXT("ThanksLightEditor"), TEXT("Thanks Light Editor"), 1280, 720
    });

    if (_appContext.get() == nullptr)
        return -1;

    TL_GameEditor::EditorApplication _app;
    _app.Start(_appContext.get());
    {
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
                _app.Tick();
            }
        }
    }
    _app.End();

    return 0;
}
