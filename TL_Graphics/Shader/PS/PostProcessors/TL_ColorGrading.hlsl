#include "TL_ConstantsPS.hlsli"

#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"



float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    float4 ret;
    
    float4 rgba = sourceSlot0.Sample(Sampler_Clamp, surface.uv);
    
    
    return float4(LUT.Sample(Sampler_Clamp, rgba.rgb).rgb, rgba.a);

}

//below LUT From 2D Image, has fundamental defection, samples between square part

//cbuffer LUT_Info : register(b10)
//{
//    uint width;
//    uint height;
//};

//float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
//{
//    float4 ret;
    
//    float4 rgba = interPass.Sample(Sampler_Wrap, surface.uv);
    
    
//    float2 rgUV = float2(rgba.r / (float) width, rgba.g / (float) height);
    
//    ret.a = rgba.a;
    
//    float depth = width * height;
//    float maxIndex = depth - 1;
    
//    float blueIndexOrigin = rgba.b * maxIndex;
//    float blueIndexFloor = floor(blueIndexOrigin);
    
//    float UIndex = blueIndexFloor % width;
//    float VIndex = (uint) blueIndexFloor / (uint) width;
    
//    float2 bUV = float2(UIndex / (float) width, VIndex / (float) height);
    
//    float2 SampleUV = rgUV + bUV;
    
//    float3 floorColor = LUT.Sample(Sampler_Wrap, SampleUV).rgb;
    
//    UIndex = (UIndex + 1) % width;
//    VIndex = (uint) (blueIndexFloor + 1) / (uint) width;
    
//    bUV = float2(UIndex / (float) width, VIndex / (float) height);
    
//    float2 SampleUV2 = rgUV + bUV;
    
//    float3 ceilColor = LUT.Sample(Sampler_Wrap, SampleUV2).rgb;
    
//    float ratio = blueIndexOrigin - blueIndexFloor;
    
//    ret.rgb = lerp(floorColor, ceilColor, ratio);
    
//    return ret;

//}