#pragma once

struct WindowData final
{
    HINSTANCE instance          = nullptr;
    HWND handle                 = nullptr;
    uint message                = 0;
    float width                 = 0.0f;
    float height                = 0.0f;
    uint monitor_width          = 0;
    uint monitor_height         = 0;
    uint virtual_monitor_width  = 0;
    uint virtual_monitor_height = 0;
    unsigned long long wparam   = 0;
    long long lparam            = 0;
};

class Engine final
{
public:
    Engine();
    ~Engine() = default;

    auto GetContext() const -> class Context* const { return context.get(); }

    auto GetWindowData() const -> const WindowData& { return window_data; }
    void SetWindowData(const WindowData& window_data);

    void Update();

private:
    std::shared_ptr<class Context> context;

    WindowData window_data;
};