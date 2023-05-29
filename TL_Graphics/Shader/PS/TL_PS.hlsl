#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

cbuffer material : register(b1)
{
    float metal;
    float rough;
}

 //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    
    
	// Sample input textures to get shading model params.
    //float3 albedo = albedoMap.;
    //float3 albedo = albedoMap.Sample(Sampler, surface.uv);
    //float metalness = matallicMap.Sample(Sampler, surface.uv);
    //float roughness = roughnessMap.Sample(Sampler, surface.uv);
    //float metalness = mMetallic.r;
    //float roughness = mRoughness.r;
    
    float3 albedo = float3(1, 1, 1);
    float metalness = metal;
    float roughness = rough;
    
    
    
    //표면으로 부터 눈으로 향하는 방향 벡터
    float3 toEye = normalize(cam.camPos - surface.pos_world);
    
    //표면의 노말
    float3 normal = surface.normal_world;
    
    // 노말과 눈방향의 각
    float nDotToEye = max(0.0, dot(normal, toEye));
    
    // 프레넬0의 값, 메탈의 경우 albedo 값으로 대체
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    
    //Light 갯수 Load from register(t11), TL_TexturesPS 확인
    uint NumLights = (uint) Lights.Load(0).x;
    
    
    // 여러 광원으로부터 직접광 (Diffuse + Specular) 더해줄 변수 선언
    float3 directLighting = 0.0f;


    for (uint i = 0; i < NumLights; ++i)
    {
        Light light = LoadLightInfo(i);
        
        if (!IsValid(light, surface.pos_world))
            continue;
        
        //빛으로 향하는 벡터
        float3 lightDir = GetLightDirection(light, surface.pos_world);
        
        //감쇠된 빛의 세기
        float3 intensity = GetLightIntensity(light, surface.pos_world);
        
        //표면과 빛의 각
        float nDotL = max(0.0f, dot(normal, lightDir));

        if (nDotL <= 0.0f)
            continue;
        
        //램버트 코사인에 의해 들어오는 조도
        float3 illuminance = intensity * nDotL;

		// 눈과 빛의 중간 벡터
        float3 halfVec = normalize(lightDir + toEye);

		// 하프벡터와 표면노멀벡터의 각
        float nDotH = max(0.0f, dot(normal, halfVec));

        // 하프벡터와 눈방향의 각
        float hDotE = max(0.0f, dot(halfVec, toEye));
        
        // 프레넬 항, 물질의 종류(F0가 바뀜), 눈과 표면(이경우 미세표면)의 각에 영향 받음
        float3 F = fresnelSchlick(F0, hDotE);
        
        
        // 분포함수, n을 노말로 갖고 있는 표면에 ,h를 노말로 갖고 있는 미세표면이 얼마나 있는지?
        // 사실 아직도 확신 못하겠음
        float D = D_Beckmann(roughness * roughness, nDotH);
        //float D = ndfGGX(cosLh, roughness);
        //float D = D_GGX(roughness * roughness, cosLh);
        
		// 기하감쇠율, masking, shadowing 으로 인한 빛의 손실율
        float G = gaSchlickGGX(nDotL, nDotToEye, roughness);

        // 빛의 굴절율, 반사율(프레넬 항)의 보수, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //금속의 경우 굴절된 빛을 모두 흡수해서 산란하지 않는다.
        float3 kd = lerp(refracted, float3(0.0f, 0.0f, 0.0f), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0f * nDotL * nDotToEye);

		// Total contribution for this light.
        directLighting = directLighting + illuminance * (diffuseBRDF + specularBRDF);

    }
    
    
	// Final fragment color.
    ret.out0 = float4(directLighting, 1.0f); //pbr
    
    //ret.out0 = float4(1, 1, 1, 1);
    
    /////////////////////////////////////////////////
    
    //float nDotL = dot(surface.normal_world, light);
    
    //ret.out1 = float4(albedo * nDotL, 1.0f); //legacy
    
    return ret;
}