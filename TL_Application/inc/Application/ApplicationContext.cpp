#include "ApplicationContext.h"

#include <shellapi.h>
#include <cxxopts.hpp>

#include "GameTime.h"
#include "imgui.h"
#include "Input.h"
#include "Window.h"

#include "Common/SystemTimer.h"
#include "Graphics/RenderSystem.h"
#include "Log/Log.h"
#include "UI/AUserInterface.h"

using PROC_CREATE_UI = TL_UI::AUserInterface* (*)(TL_Application::ApplicationContext*, const tstring&);
using PROC_RELEASE_UI = void (*)(TL_UI::AUserInterface*);

namespace TL_Application
{
    using namespace TL_Common;
    using namespace TL_Graphics;
    using namespace TL_Application;
    using namespace TL_UI;

    ApplicationContext::ApplicationContext():
        m_bEditor(false),
        m_Window(nullptr),
        m_RenderSystem(nullptr),
        m_bWindowFocus(false) { }

    ApplicationContext::~ApplicationContext()
    {
        Release(this);
    }

    std::unique_ptr<ApplicationContext> ApplicationContext::Create(ApplicationContextPreparation _preparation)
    {
        TL_Application::Window* _window = nullptr;
        TL_Graphics::RenderSystem* _renderSystem = nullptr;
        TL_UI::AUserInterface* _ui = nullptr;
        WndProcEvent _wndProcEvent;

        std::unique_ptr<ApplicationContext> _appContext{new ApplicationContext};

        SystemTimer _timer;

        if (Log::IsInitialized() == false)
        {
            Log::Initialize();
        }

        _appContext->ParseCommandLineArgs();
        _appContext->m_bEditor = _preparation.bEditor;

        if (_preparation.bHeadless == false)
        {
            // Window를 초기화합니다.

            _timer.Lap();
            Log::Core_Info("Window Start Initialize");

            _window = new Window;
            HRESULT _success = Window::GetInstance()->Initialize(
                _preparation.nCmdShow
                , _preparation.hInstance
                , _preparation.windowClassName
                , _preparation.windowName
                , _preparation.width
                , _preparation.height
            );

            if (_success != S_OK)
            {
                Log::Core_Info("Window Initialization Failed. ({0} ms)", _timer.Lap().AsMillis());
                return nullptr;
            }

            _window = Window::GetInstance();
            _appContext->m_Window = _window;
            Log::Core_Info("Window Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());

            // Application 내 몇몇 클래스 객체를 초기화합니다.

            _timer.Lap();
            Log::Core_Info("Application Classes Start Initialize");
            _appContext->m_Input = std::unique_ptr<Input>(new Input());
            _appContext->m_Input->Initialize(_window->GetHWnd());
            _appContext->m_GameTime = std::unique_ptr<GameTime>(new GameTime());
            _appContext->m_GameTime->Initialize();
            Log::Core_Info("Application Classes Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());

            // RenderSystem을 초기화합니다.

            _timer.Lap();
            Log::Core_Info("RenderSystem Start Initialize.");

            TL_Graphics::RenderSystem::Create();
            _renderSystem = TL_Graphics::RenderSystem::Get();
            _renderSystem->Init();

            _appContext->m_RenderSystem = _renderSystem;
            Log::Core_Info("RenderSystem Finish Initialize. ({0} ms)", _timer.Lap().AsMillis());

            // Windows 메세지가 발생할 때 실행될 콜백을 등록합니다.
            Window::WndProcEvent.AddListener([_renderSystem](HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
            {
                switch (_message)
                {
                    case WM_SIZE:
                    {
                        const UINT _width = LOWORD(_lParam);
                        const UINT _height = HIWORD(_lParam);
                        _renderSystem->UpdateWindowSize(_width, _height);
                        break;
                    }

                    case WM_KEYDOWN:
                    case WM_KEYUP:
                    {
                        Input::Instance->PushKeyMessage({
                            static_cast<EVirtualKey>(_wParam),
                            _message == WM_KEYDOWN ? true : false,
                            std::chrono::system_clock::now()
                        });
                        break;
                    }

                    case WM_MOUSEWHEEL:
                    {
                        const int _delta = GET_WHEEL_DELTA_WPARAM(_wParam);
                        const POINTS _pt = MAKEPOINTS(_lParam);
                        Input::Instance->PushMouseWheelMessage({_delta, Vector2(_pt.x, _pt.y)});
                        break;
                    }
                }
            });
        }

        // 생성한 AppContext를 반환합니다.
        return std::move(_appContext);
    }

    void ApplicationContext::Release(ApplicationContext* _appContext)
    {
        SystemTimer _timer;

        if (_appContext->IsHeadless() == false)
        {
            // Windows 환경일 때만 실행됩니다.

            // RenderSystem을 해제합니다.

            _timer.Lap();
            Log::Core_Info("RenderSystem Start Delete.");
            _appContext->GetRenderSystem()->Delete();
            Log::Core_Info("RenderSystem Finish Delete. ({0} ms)", _timer.Lap().AsMillis());

            // Window를 해제합니다.

            _timer.Lap();
            Log::Core_Info("Window Start Release.");
            _appContext->GetWindow()->Release();
            delete _appContext->GetWindow();
            Log::Core_Info("Window Finish Release. ({0} ms)", _timer.Lap().AsMillis());
        }
    }

    void ApplicationContext::PreTick()
    {
        m_bWindowFocus = m_Window->IsFocused();

        m_GameTime->PreTick();
        m_Input->PreTick();

        if (IsHeadless() == false)
        {
            m_RenderSystem->Clear();
            m_RenderSystem->PreRender();
            m_RenderSystem->BeginSetLight();
        }
    }

    void ApplicationContext::PreRenderObjects()
    {
        assert(IsHeadless() == false);

        m_RenderSystem->EndSetLight();
    }

    void ApplicationContext::PostRenderObjects()
    {
        assert(IsHeadless() == false);

        m_RenderSystem->PostRender();
    }

    void ApplicationContext::Present()
    {
        assert(IsHeadless() == false);

        m_RenderSystem->Present();

        // TODO: 임시적으로 코드를 이곳에 적습니다. Application을 실제로 돌리기 전 초기화 과정에서 이를 해결할 수 있는 방법을 찾으면 좋겠습니다...
        // 올바른 ImGui 동작을 위해
        // Application 실행 이후 한 번에 한해 수동으로 WM_SIZE 이벤트를 발생시킵니다.
        static bool _temp = false;
        if (_temp == false)
        {
            _temp = true;
            SetWindowPos(GetWindow()->GetHWnd(), HWND_TOP, 0, 0, 10000, 10000, SWP_NOMOVE);
            SetWindowPos(GetWindow()->GetHWnd(), HWND_TOP, 0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight(), SWP_NOMOVE);
        }
    }

    void ApplicationContext::ParseCommandLineArgs()
    {
        // command line 매개변수들을 string vector로 저장합니다.

        int _argCount;
        LPWSTR* _argList = CommandLineToArgvW(GetCommandLineW(), &_argCount);
        std::vector<std::string> _argStringList{(size_t)_argCount};
        std::vector<const char*> _argCStringList{(size_t)_argCount};
        {
            struct __deleter
            {
                void operator()(LPWSTR* ptr) { LocalFree(ptr); }
            };
            std::unique_ptr<LPWSTR, __deleter> _unique = std::unique_ptr<LPWSTR, __deleter>(_argList);

            for (int i = 0; i < _argCount; ++i)
            {
                _argStringList[i] = StringHelper::ToString(_argList[i]);
                _argCStringList[i] = _argStringList[i].c_str();
            }
        }

        // cxxopts를 사용해 string map으로 저장합니다.

        try
        {
            std::unique_ptr<cxxopts::Options> allocated(new cxxopts::Options(_argStringList[0], " - thanks light command line options"));
            auto& options = *allocated;

            options
                .add_options()
                ("app", "an application", cxxopts::value<std::string>());

            const auto result = options.parse(_argCount, _argCStringList.data());

            if (result["app"].count() > 0)
            {
                const auto _app = result["app"].as<std::string>();
                m_CommandLineArgs["app"] = _app;
            }
        }
        catch (const cxxopts::OptionException& e)
        {
            std::cout << "error parsing options: " << e.what() << std::endl;
            assert(false);
        }
    }

    TL_UI::AUserInterface* ApplicationContext::CreateUI(ApplicationContext* _appContext, const tstring& _uiType)
    {
        HMODULE _hDLL = LoadLibrary(TEXT("./TL_UI.dll"));
        assert(_hDLL != nullptr);

        auto _proc_CreateUI = (PROC_CREATE_UI)GetProcAddress(_hDLL, "CreateUI");
        auto _proc_ReleaseUI = (PROC_RELEASE_UI)GetProcAddress(_hDLL, "ReleaseUI");
        assert(_proc_CreateUI != nullptr);
        assert(_proc_ReleaseUI != nullptr);

        return _proc_CreateUI(_appContext, _uiType);
    }
}
