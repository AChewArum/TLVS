#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    return sourceSlot0.Sample(Sampler_Clamp, surface.uv);
}