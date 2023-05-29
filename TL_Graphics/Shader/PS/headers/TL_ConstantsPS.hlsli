#ifndef TL_ConstantsPS
#define TL_ConstantsPS


//00 ~ 09 : commonBuffers

struct CamInfo
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewInv;
    float4x4 projInv;
    float3 camPos;
    float frustumFar;
};

cbuffer CameraBuffer : register(b0)
{
    CamInfo cam;
}

struct TextureInfo
{
    uint texX;//as Pixel
    uint texY;
    float texXInv;//as UV
    float texYInv;
};

cbuffer RenderTargetTexInfo : register(b2)//¾²·Á³ª...? ÀÏ´Ü ¹Ì·ï³ð
{
    TextureInfo rttInfo[8]; //index means slot of renderTarget
}

cbuffer TexInfo : register(b3)
{
    TextureInfo texInfo[4096]; //index means slot of texture ref TL_TexturePS
}

cbuffer ShadowBuffer : register(b4)
{
    CamInfo shadowCamHigh;
}
cbuffer ShadowBuffer : register(b5)
{
    CamInfo shadowCamMid;
}
cbuffer ShadowBuffer : register(b6)
{
    CamInfo shadowCamLow;
}



//10 ~ 15 : define by each shader don't define slot here

#endif