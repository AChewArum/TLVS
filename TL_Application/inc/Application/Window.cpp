#include "Window.h"

#include "Common.h"

namespace TL_Application
{
    WndProcEvent Window::WndProcEvent{};

    DEFINE_SINGLETON_CLASS(Window, [](){}, [](){})

    HRESULT Window::Initialize(int _nCmdShow,
                               HINSTANCE _hInstance,
                               const tstring& _windowClassName,
                               const tstring& _windowName,
                               UINT _width,
                               UINT _height
    )
    {
        m_hInstance = _hInstance;
        m_WindowClassName = _windowClassName;
        m_WindowName = _windowName;
        m_Width = _width;
        m_Height = _height;
        m_WindowClassNameW = StringHelper::ToWString(m_WindowClassName);

        HRESULT _hr = S_OK;

        RegisterWindowClass();

        _hr = InitializeWindow(_nCmdShow);
        assert(_hr >= 0);

        _hr = ShowWindowToScreen(_nCmdShow) ? S_OK : E_FAIL;
        assert(_hr == S_OK);

        return _hr;
    }

    void Window::Release()
    {
        DestroyWindow(m_hWnd);
        UnregisterClass(m_WindowClassName.c_str(), m_hInstance);
    }

    bool Window::ShowWindowToScreen(int _nCmdShow)
    {
        // ShowWindow 함수의 반환값은 함수의 성공 여부와 관계 없습니다.
        ShowWindow(m_hWnd, _nCmdShow);

        int a = 5;

        if (UpdateWindow(m_hWnd) == false)
            return false;

        return true;
    }

    bool Window::IsFocused() const
    {
        return GetForegroundWindow() == m_hWnd;
    }

    ATOM Window::RegisterWindowClass()
    {
        // Register the window class.
        WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = Window::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = m_hInstance;
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = m_WindowClassNameW.c_str();

        const ATOM _result = RegisterClassEx(&wcex);
        return _result;
    }

    HRESULT Window::InitializeWindow(int nCmdShow)
    {
        // Create the window.
        m_hWnd = CreateWindow(
            m_WindowClassName.c_str(),
            m_WindowName.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<UINT>(m_Width),
            static_cast<UINT>(m_Height),
            NULL,
            NULL,
            m_hInstance,
            this
        );

        assert(m_hWnd != nullptr);

        return S_OK;
    }

    LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Window::WndProcEvent.Invoke(hWnd, message, wParam, lParam);

        switch (message)
        {
            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hWnd, nullptr, false);
            }
                return 0;

            case WM_PAINT:
            {
                PAINTSTRUCT _ps;
                HDC _hDC = BeginPaint(hWnd, &_ps);
                EndPaint(hWnd, &_ps);
                ValidateRect(hWnd, nullptr);
            }
                return 0;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
                return 0;
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
