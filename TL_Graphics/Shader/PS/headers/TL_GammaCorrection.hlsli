#ifndef TL_GammaCorrection
#define TL_GammaCorrection

float3 sRGBtoLinear(float3 albedo)// = Texture to world
{
    return albedo * albedo;
}

float3 LinearTosRGB(float3 albedo)// world to Texture
{
    return sqrt(albedo);
}

#endif