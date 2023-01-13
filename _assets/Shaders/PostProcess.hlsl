struct VertexInput
{
    float4 position : POSITION0;
    float2 uv       : UV0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv       : UV0;
};

cbuffer TransformBuffer : register(b0)
{
    //row_major matrix world;
    //row_major matrix view;
    //row_major matrix projection; 
    
    matrix world;
};

cbuffer CameraBuffer : register(b1)
{
	matrix view;
	matrix projection;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection); 
    output.uv       = input.uv;
    
    return output;
}

Texture2D source_texture    : register(t0);
SamplerState samp           : register(s0);

float4 PS(PixelInput input) : SV_Target
{
	return source_texture.Sample(samp, input.uv);
}