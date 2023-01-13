#pragma once
#define API_INPUT_WINDOWS
#ifdef API_INPUT_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN
#pragma comment(lib, "XInput.lib")
#include <Windows.h>
#include <Xinput.h>
#endif //API_INPUT_WINDOWS