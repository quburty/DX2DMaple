#pragma once

/*
    Design Pattern
    - Singleton Pattern
*/

class Settings final
{
public:
    static Settings& Get()
    {
        static Settings instance;
        return instance;
    }

    auto GetWindowhandle() const { return handle; }
    void SetWindowHandle(HWND const handle) { this->handle = handle; }

    auto GetWidth() const { return width; }
    void SetWidth(const float& width) { this->width = width; }

    auto GetHeight() const { return height; }
    void SetHeight(const float& height) { this->height = height; }

private:
    Settings() = default;
    ~Settings() = default;

private:
    HWND handle     = nullptr;
    float width     = 0.0f;
    float height    = 0.0f;
};