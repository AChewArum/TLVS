#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

Buffer<float4> MaterialTest : register(t50);

float2 GetMaterialByInstanceID(uint instanceID)
{
    return MaterialTest.Load(instanceID).xy;
}

PS_Out main(VS_Out surface)
{
	PS_Out ret;
	
	float4 basecolor_opacity = baseColor_opcityMap.Sample(Sampler_Wrap, surface.uv);
    
	//ret.out0 = float4(basecolor_opacity.rgb, 1.0f); //albedo
    float test = surface.instanceID / 20.0f;
    ret.out0 = float4(1.0f, 0.0f, 0.0f, 1.0f); //albedo

	ret.out1 = float4(surface.normal_world, 1.0f); //normal_world
    
    float2 mr = GetMaterialByInstanceID(surface.instanceID);
	
    float metallic = mr.x;
    float roughness = mr.y;
	
	ret.out2 = float4(surface.pos_world, 1.0f); //pos_world
    ret.out3 = float4(metallic, 1.0f, 1.0f, 0.0f); //metalness
	ret.out4 = float4(roughness, 0.0f, 0.0f, 0.0f); //roughness
	ret.out5 = float4(surface.pos_NDC.z, 0.0f, 0.0f, 0.0f);
    
	return ret;
}