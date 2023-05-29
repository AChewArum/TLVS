#include "TL_IO_VS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

float4 main(VS_Out_SkyBox surface) : SV_TARGET0
{
	return float4(skyBox.SampleLevel(Sampler_Clamp, surface.uvw, 0).rgb, 1.0f);
}