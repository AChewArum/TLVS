#ifndef TL_Light
#define TL_Light

#include "TL_TexturesPS.hlsli"

struct LightMetaData
{
    uint lightNum;
};

struct Light
{
    uint type;
    float3 position;
    
    float intensity;
    float3 direction;
    
    float range;
    float3 attenuation;

    float spot;
    float3 color;
};

static const uint LightType_Directional = 0;
static const uint LightType_Point = 1;
static const uint LightType_Spot = 2;

//const int metaDataOffset = 1;

Light LoadLightInfo(uint index)
{
    Light light;
    
    float4 type_position = Lights.Load(1 + 4 * index + 0);//1 for lightNum
    float4 intensity_direction = Lights.Load(1 + 4 * index + 1);
    float4 range_attenuation = Lights.Load(1 + 4 * index + 2);
    float4 spot_color = Lights.Load(1 + 4 * index + 3);
    
    light.type = (uint)type_position.x;
    light.position = type_position.yzw;
    
    light.intensity = intensity_direction.x;
    light.direction = intensity_direction.yzw;
    
    light.range = range_attenuation.x;
    light.attenuation = range_attenuation.yzw;
    
    light.spot = spot_color.x;
    light.color = spot_color.yzw;
    
    return light;
}

float getRangeAttenuation(float3 attenuation, float distance)
{
    
    return 1.0f / dot(attenuation, float3(1.0f, distance, distance * distance));
}

float getSpotAttenuation(Light light, float3 pos_world)
{
	float3 lightToSurface = normalize(pos_world - light.position);
    
    float lightCos = pow(saturate(dot(lightToSurface, normalize(light.direction))), light.spot);
    
	return saturate(lightCos);
}

float3 GetLightIntensity(Light light, float3 pos_world)
{
    float rangeAttenuation = 1.0;
    float spotAttenuation = 1.0;

    if (light.type != LightType_Directional)
    {
        rangeAttenuation = getRangeAttenuation(light.attenuation, length(light.position - pos_world));
    }
    if (light.type == LightType_Spot)
    {
        spotAttenuation = getSpotAttenuation(light, pos_world);
    }

    return rangeAttenuation * spotAttenuation * light.intensity * light.color;
}

//surface To Light
float3 GetLightDirection(Light light, float3 pos_world)
{
    if (light.type != LightType_Spot)
        return -normalize(light.direction);
    
    return normalize(light.position - pos_world);
}

bool IsValid(Light light, float3 pos_world)
{
    if (light.type == LightType_Directional)
        return true;
    
    //if(light.type != LightType_Directional)
    //    return length(light.position - pos_world) < light.range;
    
    //todo : if spotlight
    return true;
}


//https://github.com/KhronosGroup/glTF-Sample-Viewer/blob/master/source/Renderer/shaders/pbr.frag 참고해서 만듦

#endif