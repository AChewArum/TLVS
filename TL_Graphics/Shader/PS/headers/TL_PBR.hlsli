#ifndef TL_PBR
#define TL_PBR


 //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Constant normal incidence Fresnel factor for all dielectrics.
static const float PI = 3.141592f;
static const float3 Fdielectric = 0.04f;

static const float Epsilon = 0.00001f;

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    
    float kR = 1.0f - roughness;
    
    return F0 + (max(float3(kR, kR, kR), F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}



// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    
    //눈과 빛의 거울반사 각도가 작을 수록
    //return 값 up
    
    //roughness가 클수록
    //return 값 down
    
    return alphaSq / max(Epsilon, (PI * denom * denom));
}

float D_GGX(float a2, float NoH)
{
    
    float d = (NoH * a2 - NoH) * NoH + 1;

    return a2 / (PI * d * d);
}

//a2 = roughness^2, NoH = nDotH
float D_Beckmann(float a2, float NoH)
{
    a2 = max(Epsilon, a2); //a2 shouldnt be 0
    
    float NoH2 = NoH * NoH;
    return exp((NoH2 - 1) / (a2 * NoH2)) / (PI * a2 * NoH2 * NoH2);
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


#endif