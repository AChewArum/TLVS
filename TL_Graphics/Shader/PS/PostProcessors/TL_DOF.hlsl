#include "TL_IO_VS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"


float2 vDOFValue : POSTEFFECT_SEMANTIC_DOF_VALUE; // .x=카메라로부터 Skip할 거리, .y=가중치

#define POSTEFFECT_BLUR_SAMPLING_COUNT 9

cbuffer Dir_Info : register(b10)
{
    float2 dir;
};

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
    
    TextureInfo tex = texInfo[30];
    
    float2 step = float2(dir.x * tex.texXInv, dir.y * tex.texYInv);
    
    float4 Result = 0.0f;

    float depth = depth_Deferred.Sample(Sampler_Wrap, surface.uv);
    //float fDepthWeight = (depth > (vDOFValue.x * FarClip_rcp)) ? 1.0f : 0.0f;

    //if (depth > (vDOFValue.x * FarClip_rcp))
    //{
        for (int i = 0; i < POSTEFFECT_BLUR_SAMPLING_COUNT; i++)
        {
        Result += sourceSlot0.Sample(Sampler_Wrap, float2(surface.uv + step * indices[i])) * weights[i];
    }
    //}
    //else
    //    Result = interPass.Sample(Sampler_Wrap, surface.uv);
    
    
    return float4(Result.rgb, sourceSlot0.Sample(Sampler_Wrap, surface.uv).a);
    
    return float4(Result.rgb, 1.0f);
}
