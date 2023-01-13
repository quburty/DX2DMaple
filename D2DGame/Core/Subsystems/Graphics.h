#pragma once
#include "ISubsystem.h"

class Graphics final : public ISubsystem
{
public:
    Graphics(class Context* const context);
    ~Graphics();

public:
	void Update(const float& delta_time_sec) override;

    bool Initialize() override;
    void CreateBackBuffer(const uint& width, const uint& height);

    auto GetDevice() const { return device; }
    auto GetDeviceContext() const { return device_context; }

    void Begin();
    void End();

private:
    ID3D11Device* device                        = nullptr;
    ID3D11DeviceContext* device_context         = nullptr;
    IDXGISwapChain* swap_chain                  = nullptr;
    
    ID3D11RenderTargetView* render_target_view  = nullptr;
    D3D11_VIEWPORT viewport;
    D3DXCOLOR clear_color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
};