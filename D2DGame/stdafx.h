#pragma once
//precompiled header

#pragma warning(disable : 4506) //https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4506?view=vs-2017

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define NOMINMAX

/*
    Window
*/
#include <Windows.h>
#include <cassert>

/*
    STL
*/
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>
#include <random>
#include <chrono>
#include <mutex>
#include <thread>
#include <any> //<- ????
#include <tuple>
#include <deque>

/*
    DirectX
*/
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

/*
    typedef
*/
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

/*
    macro function
*/
#define SAFE_DELETE(p)          { if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)    { if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)         { if (p) { (p)->Release(); (p) = nullptr; } }
#define TYPE_STR(t) typeid(t).name()

/*
    Log
*/
#include "Log/Log.h"

/*
    Math
*/
#include "Math/Math.h"

/*
    Utility
*/
#include "Utility/FileStream.h"
#include "Utility/FileSystem.h"
#include "Utility/Geometry_Generator.h"
#include "Utility/GUID_Generator.h"
#include "Utility/Xml.h"

/*
    Main System
*/
#include "Core/Settings.h"
#include "Core/Context.h"
#include "Core/Engine.h"

/*
    Subsystem
*/
#include "Core/Subsystems/Timer.h"
#include "Core/Subsystems/Input/Input.h"
#include "Core/Subsystems/Audio.h"
#include "Core/Subsystems/Thread.h"
#include "Core/Subsystems/Graphics.h"
#include "Core/Subsystems/ResourceManager.h"
#include "Core/Subsystems/Renderer.h"
#include "Core/Subsystems/GameData.h"
#include "Core/Subsystems/Scene.h"

/*
    D3D11 Wrapper class
*/
#include "Core/Object.h"
#include "Core/D3D11/Vertex/D3D11_Vertex.h"
#include "Core/D3D11/Vertex/D3D11_Geometry.h"
#include "Core/D3D11/InputAssembler/D3D11_VertexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_IndexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_InputLayout.h"
#include "Core/D3D11/Shader/D3D11_Shader.h"
#include "Core/D3D11/Shader/D3D11_ConstantBuffer.h"
#include "Core/D3D11/Shader/D3D11_Texture.h"
#include "Core/D3D11/Shader/D3D11_SamplerState.h"
#include "Core/D3D11/Rasterizer/D3D11_RasterizerState.h"
#include "Core/D3D11/OutputMerger/D3D11_BlendState.h"
#include "Core/D3D11/D3D11_Pipeline.h"

/*
    Resource
*/
#include "Resource/Animation.h"
#include "Resource/AudioClip.h"
#include "Resource/Texture2D.h"
