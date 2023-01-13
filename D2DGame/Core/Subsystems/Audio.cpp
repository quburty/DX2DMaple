#include "stdafx.h"
#include "Audio.h"

#pragma warning(push, 0)
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma warning(pop)

Audio::Audio(Context * const context)
    : ISubsystem(context)
{
}

Audio::~Audio()
{
    listener = nullptr;

    if (!system)
        return;

    result = system->close();
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return;
    }

    result = system->release();
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return;
    }
}

bool Audio::Initialize()
{
    result = System_Create(&system);
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return false;
    }

    uint version = 0;
    result = system->getVersion(&version);
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return false;
    }

    if (version < FMOD_VERSION)
    {
        LogErrorHandler(result);
        return false;
    }

    auto driver_count = 0;
    result = system->getNumDrivers(&driver_count);
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return false;
    }

    //TODO : check driver count

    result = system->init(max_channels, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return false;
    }

    result = system->set3DSettings(1.0f, distance_factor, 0.0f);
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return false;
    }

    is_initialized = true;

    return true;
}

void Audio::Update(const float & delta_time_sec)
{
    if (!is_initialized)
        return;

    result = system->update();
    if (result != FMOD_OK)
    {
        LogErrorHandler(result);
        return;
    }

    /*if (listener)
    {
        auto position = listener->GetPosition();
        auto velocity = D3DXVECTOR3{0.0f, 0.0f, 0.0f};
        auto forward = listener->GetForward();
        auto up = listener->GetUp();

        result = system->set3DListenerAttributes
        (
            0,
            reinterpret_cast<FMOD_VECTOR*>(&position),
            reinterpret_cast<FMOD_VECTOR*>(&velocity),
            reinterpret_cast<FMOD_VECTOR*>(&forward),
            reinterpret_cast<FMOD_VECTOR*>(&up)
        );

        if (result != FMOD_OK)
        {
            LogErrorHandler(result);
            return;
        }
    }*/
}

void Audio::LogErrorHandler(const int & error) const
{
    LOG_ERROR(std::string(FMOD_ErrorString(static_cast<FMOD_RESULT>(error))));
}
