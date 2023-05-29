#include "TL_IO_VS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
    return float4(sourceSlot0.Sample(Sampler_Clamp, surface.uv) + sourceSlot1.Sample(Sampler_Clamp, surface.uv).rgb, 1.0f);
}