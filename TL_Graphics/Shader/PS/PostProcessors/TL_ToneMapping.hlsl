#include "TL_ConstantsPS.hlsli"

#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Reinhard.hlsli"
// ReinHard ToneMapping, https://64.github.io/tonemapping/
// + DirectX Sample HDRToneMapping

cbuffer Dir_Info : register(b10)
{
    float middleGrey;
    float maxWhite;
};

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    //float4 vColor = 0;
    float4 vColor = sourceSlot0.Sample(Sampler_Clamp, surface.uv);
    
    float luminance = Luminance(vColor.rgb);
    
    luminance = Reinhard_Luminance(luminance, 7.0f);
    
    vColor.rgb = Change_luminance(vColor.rgb, luminance);
    
    
    vColor.a = 1.0f;
    
    return vColor;

}
