#pragma once

class D3D11_InputLayout final : public Object
{
public:
    D3D11_InputLayout(class Context* const context);
    ~D3D11_InputLayout();

    auto GetResource() const -> ID3D11InputLayout* const { return input_layout; }

    void Create(const D3D11_INPUT_ELEMENT_DESC* const descs, const uint& count, ID3DBlob* const blob);
	void Create(ID3D10Blob* const blob);
	void Clear();

private:
    ID3D11Device* device            = nullptr;
    ID3D11InputLayout* input_layout = nullptr;
};