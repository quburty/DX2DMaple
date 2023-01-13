#include "stdafx.h"
#include "Core/Window.h"
#include "Core/Engine.h"

int APIENTRY WinMain
(
    HINSTANCE hInstance,
    HINSTANCE prevInstance,
    LPSTR lpszCmdParam,
    int nCmdShow
)
{
    std::cout << GUID_Generator::Generator() << std::endl;

    Window::Create(hInstance, 1280, 720);
    Window::Show();

    Settings::Get().SetWindowHandle(Window::handle);
    Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
    Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));

    auto engine = std::make_unique<Engine>();

    Window::on_event = [&engine](const WindowData& window_data) 
    {
        engine->SetWindowData(window_data);
    };

    while (Window::Update())
    {
        engine->Update();
    }

    Window::Destroy();
    return 0;
}