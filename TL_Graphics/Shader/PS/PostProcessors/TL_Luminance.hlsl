#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Reinhard.hlsli"

///픽셀의 휘도를 구하는 쉐이더, GreyScale과 같다고 볼 수 있을 듯.

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    return float4(Luminance(sourceSlot0.Sample(Sampler_Clamp, surface.uv).rgb).r, 0.0f, 0.0f, 1.0f);
}