#pragma once

#include "Common_min.h"
#include "Struct/Event.h"
#include "Application/internal/dll.h"

namespace TL_Application
{
    using WndProcEvent = TL_Common::Event<HWND, UINT, WPARAM, LPARAM>;
    using PreShowWindowEvent = TL_Common::Event<>;

    class APPLICATION_API Window
    {
        DECLARE_SINGLETON_CLASS(Window)

    public:
        HRESULT Initialize(int _nCmdShow,
                           HINSTANCE _hInstance,
                           const tstring& _windowClassName,
                           const tstring& _windowName,
                           UINT _width,
                           UINT _height
        );

        void Release();

        /**
         * @brief 화면에 메인 윈도우를 표시합니다.
         */
        bool ShowWindowToScreen(int _nCmdShow);

        bool IsFocused() const;

    private:
        /**
         * @brief 윈도우 클래스를 등록합니다.
         */
        ATOM RegisterWindowClass();

        /**
         * @brief 프로그램에서 사용할 메인 윈도우를 생성합니다.
         */
        HRESULT InitializeWindow(int nCmdShow);

    private:
        tstring m_WindowClassName;
        tstring m_WindowName;
        std::wstring m_WindowClassNameW;

        /**
         * @brief 프로세스 인스턴스 핸들입니다.
         */
        HINSTANCE m_hInstance;

        /**
         * @brief 메인 윈도우의 핸들입니다.
         */
        HWND m_hWnd;

        UINT m_Width;

        UINT m_Height;

    public:
        HINSTANCE GetHInstance() const { return m_hInstance; }
        HWND GetHWnd() const { return m_hWnd; }
        UINT GetWidth() const { return m_Width; }
        UINT GetHeight() const { return m_Height; }

    public:
        /**
         * @brief 윈도우 메세지를 처리하는 윈도우 콜백 함수입니다.
         */
        static LRESULT CALLBACK WndProc(
            HWND hWnd,
            UINT message,
            WPARAM wParam,
            LPARAM lParam
        );

        static WndProcEvent WndProcEvent;
        static PreShowWindowEvent PreShowWindowEvent;
    };
}
