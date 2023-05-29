#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

static const float MIDDLE_GRAY = 0.15f;

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    float4 original = sourceSlot0.Sample(Sampler_Clamp, surface.uv);
    float luminance = max(0.001f, sourceSlot1.Sample(Sampler_Clamp, float2(0.5f, 0.5f)));
	
    float3 ret = original * MIDDLE_GRAY / luminance;
    
    return float4(ret, 1.0f);
}