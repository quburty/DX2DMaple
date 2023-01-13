#include "stdafx.h"
#include "D3D11_Pipeline.h"

D3D11_Pipeline::D3D11_Pipeline(Context * const context)
{
    device_context = context->GetSubsystem<Graphics>()->GetDeviceContext();
}

bool D3D11_Pipeline::Begin(const D3D11_PipelineState & pipeline_state)
{
    //InputLayout
    {
        if (pipeline_state.vertex_shader)
        {
            if (auto input_layout=pipeline_state.vertex_shader->GetInputLayout())
            {
				if (auto resource = input_layout->GetResource())
					device_context->IASetInputLayout(resource);
            }
        }
    }

    //Primitive Topology
    {
        if (bind_primitive_topology_id != static_cast<uint>(pipeline_state.primitive_topology))
        {
            device_context->IASetPrimitiveTopology(pipeline_state.primitive_topology);
            bind_primitive_topology_id = static_cast<uint>(pipeline_state.primitive_topology);
        }
    }

    //Vertex Shader
    {
        if (pipeline_state.vertex_shader)
        {
            if (auto new_shader = pipeline_state.vertex_shader->GetResource())
            {
                uint instance_count{ 256 };
                ID3D11ClassInstance* instance[256]{ nullptr };
                ID3D11VertexShader* current_shader{ nullptr };
                device_context->VSGetShader(&current_shader, instance, &instance_count);

                if (current_shader != new_shader)
                {
                    device_context->VSSetShader(static_cast<ID3D11VertexShader*>(new_shader), nullptr, 0);

                    bind_vertex_buffer_id   = 0;
                    bind_index_buffer_id    = 0;
                }
            }
        }
    }

    //Pixel Shader
    {
        uint instance_count{ 256 };
        ID3D11ClassInstance* instance[256]{ nullptr };
        ID3D11PixelShader* current_shader{ nullptr };
        device_context->PSGetShader(&current_shader, instance, &instance_count);

        auto new_shader = pipeline_state.pixel_shader ? pipeline_state.pixel_shader->GetResource() : nullptr;
        if (current_shader != new_shader)
            device_context->PSSetShader(static_cast<ID3D11PixelShader*>(new_shader), nullptr, 0);
    }

    //Rasterizer State
    {
        if (pipeline_state.rasterizer_state && pipeline_state.rasterizer_state->GetResource())
        {
            if (bind_rasterizer_state_id != pipeline_state.rasterizer_state->GetID())
            {
                device_context->RSSetState(pipeline_state.rasterizer_state->GetResource());
                bind_rasterizer_state_id = pipeline_state.rasterizer_state->GetID();
            }
        }
    }

    //Blend State
    {
        if (pipeline_state.blend_state && pipeline_state.blend_state->GetResource())
        {
            if (bind_blend_state_id != pipeline_state.blend_state->GetID())
            {
                float factor            = pipeline_state.blend_state->GetBlendFactor();
                float blend_factor[]    = { factor, factor, factor, factor };
                uint sample_mask        = pipeline_state.blend_state->GetSampleMask();

                device_context->OMSetBlendState(pipeline_state.blend_state->GetResource(), blend_factor, sample_mask);
                bind_blend_state_id = pipeline_state.blend_state->GetID();
            }
        }
    }

    return true;
}

void D3D11_Pipeline::End()
{
    //TODO :
}

void D3D11_Pipeline::SetVertexBuffer(const D3D11_VertexBuffer * const buffer)
{
    if (!buffer || !buffer->GetResource())
    {
        assert(false);
        return;
    }

    if (bind_vertex_buffer_id == buffer->GetID())
        return;

    ID3D11Buffer* buffers[]{ buffer->GetResource() };
    device_context->IASetVertexBuffers(0, 1, buffers, &buffer->GetStride(), &buffer->GetOffset());
    bind_vertex_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetIndexBuffer(const D3D11_IndexBuffer * const buffer)
{
    if (!buffer || !buffer->GetResource())
    {
        assert(false);
        return;
    }

    if (bind_index_buffer_id == buffer->GetID())
        return;

    device_context->IASetIndexBuffer(buffer->GetResource(), DXGI_FORMAT_R32_UINT, buffer->GetOffset());
    bind_index_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetConstantBuffer(const uint & slot, const uint & scope, const D3D11_ConstantBuffer * const buffer)
{
    if (slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11Buffer* constant_buffers[]{ buffer ? buffer->GetResource() : nullptr };
    
    if (scope & ShaderScope_VS) device_context->VSSetConstantBuffers(slot, 1, constant_buffers);
    if (scope & ShaderScope_PS) device_context->PSSetConstantBuffers(slot, 1, constant_buffers);
}

void D3D11_Pipeline::SetShaderResource(const uint & slot, const uint & scope, const D3D11_Texture * const resource)
{
    if (slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11ShaderResourceView* shader_resources[]{ resource ? resource->GetResource_SRV() : nullptr };

    if (scope & ShaderScope_VS) device_context->VSSetShaderResources(slot, 1, shader_resources);
    if (scope & ShaderScope_PS) device_context->PSSetShaderResources(slot, 1, shader_resources);
}

void D3D11_Pipeline::SetSamplerState(const uint & slot, const uint & scope, const D3D11_SamplerState * const state)
{
    if (slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT)
    {
        assert(false);
        return;
    }

    ID3D11SamplerState* sampler_states[]{ state ? state->GetResource() : nullptr };

    if (scope & ShaderScope_VS) device_context->VSSetSamplers(slot, 1, sampler_states);
    if (scope & ShaderScope_PS) device_context->PSSetSamplers(slot, 1, sampler_states);
}


void D3D11_Pipeline::DrawIndexed(const uint & index_count, const uint & index_offset, const uint & vertex_offset)
{
    device_context->DrawIndexed(index_count, index_offset, vertex_offset);
}
