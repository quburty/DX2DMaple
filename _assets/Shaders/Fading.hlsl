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
    matrix world;
};

cbuffer CameraBuffer : register(b1)
{
	matrix view;
	matrix projection;
};

cbuffer AnimationBuffer : register(b2)
{
	float2 sprite_texture_size;
	float2 offset;
	float2 size;
    float time;
    float alpha;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

	output.position = float4(input.position.xy * size.xy, 0.0f, 1.0f);
    output.position = mul(output.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection); 
    output.uv       = input.uv;
	output.uv      *= size / sprite_texture_size;
	output.uv      += offset / sprite_texture_size;
    
    return output;
}

Texture2D source_texture    : register(t0);
SamplerState samp           : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0.0f;
    color = source_texture.Sample(samp, input.uv);
    color.a *= saturate(1 - time);
    
    return color;
}