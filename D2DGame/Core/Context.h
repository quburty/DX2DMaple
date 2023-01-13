#pragma once
#include "stdafx.h"
#include "Subsystems/ISubsystem.h"

class Context final
{
public:
    Context() = default;
    ~Context()
    {
        for (auto iter = subsystems.rbegin(); iter != subsystems.rend(); iter++)
            SAFE_DELETE(*iter);
    }

    bool Initialize_subsystems()
    {
        auto result = true;
        for (const auto& subsystem : subsystems)
        {
            if (!subsystem->Initialize())
            {
                LOG_ERROR_F("Failed to initialized %s", typeid(*subsystem).name());
                result = false;
            }
        }
        return result;
    }

    void Update_subsystems(const float& delta_time)
    {
        for (const auto& subsystem : subsystems)
            subsystem->Update(delta_time);
    }

    template <typename T>
    auto Register_subsystem() -> T* const;

    template <typename T>
    auto GetSubsystem() -> T* const;

    auto GetEngine() const -> class Engine* const { return engine; }
    void SetEngine(class Engine* const engine) { this->engine = engine; }

private:
    class Engine* engine = nullptr;
    std::vector<ISubsystem*> subsystems;
};

template<typename T>
inline auto Context::Register_subsystem() -> T * const
{
    static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");
   
    return static_cast<T*>(subsystems.emplace_back(new T(this)));
}

template<typename T>
inline auto Context::GetSubsystem() -> T * const
{
    static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");

    for (const auto& subsystem : subsystems)
    {
        if (typeid(T) == typeid(*subsystem))
            return static_cast<T*>(subsystem);
    }
    return nullptr;
}
