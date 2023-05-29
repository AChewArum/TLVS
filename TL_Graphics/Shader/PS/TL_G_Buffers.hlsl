#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"


PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    float4 basecolor_opacity = baseColor_opcityMap.Sample(Sampler_Wrap, surface.uv);
    
    //float4 basecolor_opacity = float4(0, 0, 0, 1);
    
    float4 r_s_m_ao = roughness_specular_metallic_AOMap.Sample(Sampler_Wrap, surface.uv);
    
    
    //normal Mapping
    float3 normal = normalMap.Sample(Sampler_Wrap, surface.uv);
    
    normal = (normal * 2.0f) - 1.0f;
    
    float3x3 TBN = float3x3(surface.tangent_world, surface.bitangent_world, surface.normal_world);
    
    normal = normalize(mul(normal, TBN));
    ///
    
    float3 emissive = emissiveMap.Sample(Sampler_Clamp, surface.uv);
    
    //todo : 추후 opacity 추가할 것
    //ret.out0 = basecolor_opacity; //albedo
    ret.out0 = float4(basecolor_opacity.rgb, 1.0f); //albedo

    ret.out1 = float4(normal, 1.0f); //normal_world
    
    ret.out2 = float4(surface.pos_world, 1.0f); //pos_world
    ret.out3 = float4(r_s_m_ao.b, 1.0f, 1.0f, 0.0f); //metalness
    ret.out4 = float4(r_s_m_ao.r, 0.0f, 0.0f, 0.0f); //roughness
    ret.out5 = float4(surface.pos_NDC.z, 0.0f, 0.0f, 0.0f);
    ret.out6 = float4(surface.linearDepth, 0.0f, 0.0f, 0.0f);
    ret.out7 = float4(emissive, 0.0f);
    
    return ret;
}