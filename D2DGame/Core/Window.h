#pragma once
#include "stdafx.h"

namespace Window
{
    static HINSTANCE instance;
    static HWND handle;
    static std::function<void(const WindowData&)> on_event;

    inline const uint GetWidth()
    {
        RECT rect;
        ::GetClientRect(handle, &rect);
        return static_cast<uint>(rect.right - rect.left);
    }

    inline const uint GetHeight()
    {
        RECT rect;
        ::GetClientRect(handle, &rect);
        return static_cast<uint>(rect.bottom - rect.top);
    }

    inline LRESULT CALLBACK WndProc
    (
        HWND handle,
        uint message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        WindowData window_data;
        window_data.instance                = instance;
        window_data.handle                  = handle;
        window_data.message                 = message;
        window_data.wparam                  = static_cast<unsigned long long>(wParam);
        window_data.lparam                  = static_cast<long long>(lParam);
        window_data.monitor_width           = static_cast<uint>(GetSystemMetrics(SM_CXSCREEN));
        window_data.monitor_height          = static_cast<uint>(GetSystemMetrics(SM_CYSCREEN));
        window_data.virtual_monitor_width   = static_cast<uint>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
        window_data.virtual_monitor_height  = static_cast<uint>(GetSystemMetrics(SM_CYVIRTUALSCREEN));
        window_data.width                   = static_cast<float>(GetWidth());
        window_data.height                  = static_cast<float>(GetHeight());

        if (on_event)
            on_event(window_data);

        switch (message)
        {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(handle, message, wParam, lParam);
        }

        return 0;
    }

    inline void Create(HINSTANCE hInstance, const uint& width, const uint& height)
    {
        instance = hInstance;

        WNDCLASSEXA wnd_class;
        wnd_class.cbClsExtra    = 0;
        wnd_class.cbWndExtra    = 0;
        wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wnd_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
        wnd_class.hIconSm       = LoadIcon(nullptr, IDI_WINLOGO);
        wnd_class.hInstance     = hInstance;
        wnd_class.lpfnWndProc   = WndProc;
        wnd_class.lpszClassName = "D2DGame";
        wnd_class.lpszMenuName  = nullptr;
        wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
        wnd_class.cbSize        = sizeof(WNDCLASSEXA);

        auto check = RegisterClassExA(&wnd_class);
        assert(check != 0);

        handle = CreateWindowExA
        (
            WS_EX_APPWINDOW,
            "D2DGame",
            "D2DGame",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<int>(width),
            static_cast<int>(height),
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );
        assert(handle != nullptr);
    }

    inline void Show()
    {
        ::SetForegroundWindow(handle);
        ::SetFocus(handle);
        //::ShowCursor(TRUE);
		::ShowCursor(FALSE);
        ::ShowWindow(handle, SW_SHOWNORMAL);
        ::UpdateWindow(handle);
    }

    inline const bool Update()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return msg.message != WM_QUIT;
    }

    inline void Destroy()
    {
        DestroyWindow(handle);
        UnregisterClassA("D2DGame", instance);
    }
}