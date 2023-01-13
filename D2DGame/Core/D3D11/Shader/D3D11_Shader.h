#pragma once

enum ShaderScope : uint
{
    ShaderScope_Unknown = 0U,
    ShaderScope_VS      = 1U << 0,
    ShaderScope_PS      = 1U << 1,
};

enum class CompilationState : uint
{
    Unknown,
    Compiling,
    Succeeded,
    Failed,
};

class D3D11_Shader final : public Object
{
public:
    D3D11_Shader(class Context* const context);
    ~D3D11_Shader();

    auto GetResource()          const -> void* const { return shader; }
	auto GetInputLayout()		const -> const std::shared_ptr<class D3D11_InputLayout>& { return input_layout; }
    auto GetBlob()              const -> ID3DBlob* const { return blob; }
    auto GetName()              const -> const std::string& { return name; }
    auto GetPath()              const -> const std::string& { return path; }
    auto GetScope()             const -> const ShaderScope& { return shader_scope; }
    auto GetCompilationState()  const -> const CompilationState& { return compilation_state; }

    auto GetEntryPoint()        const -> const char* const;
    auto GetTargetProfile()     const -> const char* const;
    auto GetShaderModel()       const -> const char* const;

    bool IsCompiled()           const { return compilation_state == CompilationState::Succeeded; }

    void Create(const ShaderScope& scope, const std::string& path);
    void Clear();

private:
    auto Compile(const ShaderScope& scope, const std::string& path) -> void* const;

private:
	Context * context = nullptr;
    ID3D11Device* device                = nullptr;
    ID3DBlob* blob                      = nullptr;
    void* shader                        = nullptr;

    std::string name                    = "";
    std::string path                    = "";
    ShaderScope shader_scope            = ShaderScope_Unknown;
    CompilationState compilation_state  = CompilationState::Unknown;

	std::shared_ptr<class D3D11_InputLayout> input_layout;
};