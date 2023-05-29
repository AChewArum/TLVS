#include "TL_IO_VS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#define POSTEFFECT_BLUR_SAMPLING_COUNT 9


cbuffer Dir_Info : register(b10)
{
    float2 dir;
};

// X방향 Gaussian Blur
float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
    float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = { -4, -3, -2, -1, 0, +1, +2, +3, +4 };
    
    
    TextureInfo tex = texInfo[31];//interPass SlotNumber
    
    float2 step = float2(dir.x * tex.texXInv, dir.y * tex.texYInv) * 3.0f;
    
    float4 Result = 0.0f;
    
    for (int i = 0; i < POSTEFFECT_BLUR_SAMPLING_COUNT; i++)
    {
        //너무 작은 수 들어가니까 NaN 떠서 0.001f로 함
        Result += max(0.001f, sourceSlot0.Sample(Sampler_Clamp, float2(surface.uv + step * indices[i]))) * weights[i];
    }
    
    
    return float4(Result.rgb, 1.0f);
}
