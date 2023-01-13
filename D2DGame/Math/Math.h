#pragma once

class Math final
{
public:
    static constexpr float PI       = 3.14159265359f;
    static constexpr float PI_2     = 6.28318530718f;
    static constexpr float PI_DIV_2 = 1.57079632679f;
    static constexpr float PI_DIV_4 = 0.78539816339f;
    static constexpr float TO_DEG   = 180.0f / PI;
    static constexpr float TO_RAD   = PI / 180.0f;

    static auto Random(const float& min, const float& max) -> const float;

    template <typename T>
    static constexpr T Clamp(const T& val, const T& min, const T& max)
    {
        return (val < min) ? min : (val > max) ? max : val;
    }
};