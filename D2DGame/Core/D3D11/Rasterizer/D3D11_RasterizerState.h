#pragma once

class D3D11_RasterizerState final : public Object
{
public:
    D3D11_RasterizerState(class Context* const Context);
    ~D3D11_RasterizerState();

    auto GetResource() const -> ID3D11RasterizerState* const { return state; }

    void Create
    (
        const D3D11_CULL_MODE& cull_mode,
        const D3D11_FILL_MODE& fill_mode
    );
    void Clear();

private:
    ID3D11Device* device            = nullptr;
    ID3D11RasterizerState* state    = nullptr;
};