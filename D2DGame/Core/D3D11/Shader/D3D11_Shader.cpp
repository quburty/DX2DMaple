#include "stdafx.h"
#include "D3D11_Shader.h"

D3D11_Shader::D3D11_Shader(class Context* const context)
{
	this->context = context;
	device = context->GetSubsystem<Graphics>()->GetDevice();
}

D3D11_Shader::~D3D11_Shader()
{
    Clear();
}

auto D3D11_Shader::GetEntryPoint() const -> const char * const
{
    static const char* const entry_point_empty  = nullptr;
    static const char* const entry_point_vs     = "VS";
    static const char* const entry_point_ps     = "PS";

    switch (shader_scope)
    {
    case ShaderScope_VS: return entry_point_vs;
    case ShaderScope_PS: return entry_point_ps;
    }

    return entry_point_empty;
}

auto D3D11_Shader::GetTargetProfile() const -> const char * const
{
    static const char* const target_profile_empty = nullptr;

    static const char* const target_profile_vs = "vs_5_0";
    static const char* const target_profile_ps = "ps_5_0";

    switch (shader_scope)
    {
    case ShaderScope_VS: return target_profile_vs;
    case ShaderScope_PS: return target_profile_ps;
    }

    return target_profile_empty;
}

auto D3D11_Shader::GetShaderModel() const -> const char * const
{
    static const char* const shader_model = "5_0";

    return shader_model;
}

void D3D11_Shader::Create(const ShaderScope & scope, const std::string & path)
{
    shader_scope = scope;

    compilation_state = CompilationState::Compiling;
    shader = Compile(scope, path);
    compilation_state = shader ? CompilationState::Succeeded : CompilationState::Failed;
}

void D3D11_Shader::Clear()
{
    switch (shader_scope)
    {
    case ShaderScope_VS:
    {
        auto vertex_shader = static_cast<ID3D11VertexShader*>(shader);
        SAFE_RELEASE(vertex_shader);
        break;
    }
    case ShaderScope_PS:
    {
        auto pixel_shader = static_cast<ID3D11PixelShader*>(shader);
        SAFE_RELEASE(pixel_shader);
        break;
    }
    }

    SAFE_RELEASE(blob);
    name                = "";
    path                = "";
    shader_scope        = ShaderScope_Unknown;
    compilation_state   = CompilationState::Unknown;  
}

auto D3D11_Shader::Compile(const ShaderScope & scope, const std::string & path) -> void * const
{
    ID3DBlob* error = nullptr;
    auto hr = D3DX11CompileFromFileA
    (
        path.c_str(),
        nullptr,
        nullptr,
        GetEntryPoint(),
        GetTargetProfile(),
        0,
        0,
        nullptr,
        &blob,
        &error,
        nullptr
    );

    if (error)
    {
        auto str = static_cast<const char*>(error->GetBufferPointer());
        MessageBoxA(nullptr, str, "Shader Error!!", MB_OK);
    }

    assert(SUCCEEDED(hr));

    void* resource = nullptr;
    if (blob)
    {
        switch (shader_scope)
        {
        case ShaderScope_VS:
            hr = device->CreateVertexShader
            (
                blob->GetBufferPointer(),
                blob->GetBufferSize(),
                nullptr,
                reinterpret_cast<ID3D11VertexShader**>(&resource)
            );
            assert(SUCCEEDED(hr));

			if (!input_layout)
			{
				input_layout = std::make_shared<D3D11_InputLayout>(context);
				input_layout->Create(blob);
			}
			else
			{
				input_layout->Clear();
				input_layout->Create(blob);
			}

            break;
        case ShaderScope_PS:
            hr = device->CreatePixelShader
            (
                blob->GetBufferPointer(),
                blob->GetBufferSize(),
                nullptr,
                reinterpret_cast<ID3D11PixelShader**>(&resource)
            );
            assert(SUCCEEDED(hr));
            break;
        }
    }

    return resource;
}
