
cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float3 camPos;
}

cbuffer Color : register(b1)
{
    float4 color;
    float4 roughness;
    float4 metallic;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 posW : POSITIONW;
};

struct PSout
{
    float4 out0 : SV_Target0;
    float4 out1 : SV_Target1;
};

// Constant normal incidence Fresnel factor for all dielectrics.
static const float PI = 3.141592;
static const float3 Fdielectric = 0.04;

static const uint NumLights = 1;
static const float Epsilon = 0.00001;

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}


PSout main(VSout input)
{
    //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
	// Sample input textures to get shading model params.
    float3 albedo = color.rgb;
    float metalness = roughness.r;
    float roughness = metallic. r;
    
	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 Lo = normalize(camPos - input.posW);
    
    // Get current fragment's normal and transform to world space.
    float3 N = input.normal;
    
    // Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));
    
    // Specular reflection vector.
    float3 Lr = 2.0 * cosLo * N - Lo;
    
    // Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    // Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0;
    
    float3 light = float3(0, 0, -1);
    
    for (uint i = 0; i < NumLights; ++i)
    {
        float3 Li = light;
        float3 Lradiance = float3(1, 1, 1);

		// Half-vector between Li and Lo.
        float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));

		// Calculate Fresnel term for direct lighting. 
        float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
        float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
        float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }
    
    
    PSout result;
    
	// Final fragment color.
    result.out0 = float4(directLighting, 1.0);//pbr
    
    /////////////////////////////////////////////////
    
    
    float nDotL = dot(input.normal, light);
    
    result.out1 = float4(albedo * nDotL, 1.0f);//legacy
    
    return result;
}