#include "stdafx.h"
#include "Engine.h"

Engine::Engine()
{
    context = std::make_shared<Context>();
    context->SetEngine(this);

    context->Register_subsystem<Timer>();
    context->Register_subsystem<Input>();
    context->Register_subsystem<Audio>();
	context->Register_subsystem<Thread>();
    context->Register_subsystem<Graphics>();
	context->Register_subsystem<ResourceManager>();
	context->Register_subsystem<Renderer>();
	context->Register_subsystem<GameData>();
	context->Register_subsystem<Scene>();

    context->Initialize_subsystems();
}

void Engine::SetWindowData(const WindowData & window_data)
{
    this->window_data = window_data;
}

void Engine::Update()
{
    static Timer* timer = context->GetSubsystem<Timer>();

	auto graphics = context->GetSubsystem<Graphics>();
	

	graphics->Begin();
    context->Update_subsystems(timer->GetDeltaTimeSEC());

	auto renderer = context->GetSubsystem<Renderer>();

	graphics->End();
}