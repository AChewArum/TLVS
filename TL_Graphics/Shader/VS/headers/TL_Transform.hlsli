#ifndef TL_Transform
#define TL_Transform

#include "TL_ConstantsVS.hlsli"

#include "TL_TexturesVS.hlsli"

///버텍스용
float3 LocalToWorld(float3 localPos)//로컬 포지션을 월드로
{
	return mul(world, float4(localPos, 1.0f)).xyz;
}

float3 WorldToView(float3 worldPos)
{
    return mul(view, float4(worldPos, 1.0f)).xyz;
}

float4 ViewToNDC(float3 viewPos)
{
    return mul(proj, float4(viewPos, 1.0f));
}

float4 WorldToNDC(float3 worldPos)
{
    float3 pos_view = WorldToView(worldPos).xyz;
    
    return mul(proj, float4(pos_view, 1.0f));
}
float4 LocalToNDC(float3 localPos)//로컬 포지션을 월드로
{
    float3 pos_world = LocalToWorld(localPos);
    
	return WorldToNDC(pos_world);
}

///벡터용
float3 LocalToWorldDirOnly(float3 localDir)//로컬 포지션을 월드로
{
    return mul((float3x3) world, localDir);
}

float3 WorldToNDCDirOnly(float3 worldDir)
{
    float3 ret = mul((float3x3) view, worldDir);
    ret = mul((float3x3) proj, ret);
    
    return ret;
}

float3 LocalToNDCDirOnly(float3 localDir)//로컬 포지션을 월드로
{
    float3 ret = LocalToWorldDirOnly(localDir);
    ret = WorldToNDCDirOnly(ret);
    
    return ret;
}

//인스턴싱

float4x4 GetInstanceWorldTransform(uint instanceID)
{
    float4 r0 = Transforms.Load(0 + 4 * instanceID);
    float4 r1 = Transforms.Load(1 + 4 * instanceID);
    float4 r2 = Transforms.Load(2 + 4 * instanceID);
    float4 r3 = Transforms.Load(3 + 4 * instanceID);
    
    return float4x4(r0, r1, r2, r3);
}

float3 LocalToWorld(float3 localPos, uint instanceID)//로컬 포지션을 월드로
{
    return mul(float4(localPos, 1.0f), GetInstanceWorldTransform(instanceID)).xyz;
}

float4 LocalToNDC(float3 localPos, uint instanceID)//로컬 포지션을 월드로
{
    float3 ret = LocalToWorld(localPos, instanceID);
    
    return WorldToNDC(ret);
}

///벡터용
float3 LocalToWorldDirOnly(float3 localDir, uint instanceID)//로컬 포지션을 월드로
{
    return mul(localDir, (float3x3) GetInstanceWorldTransform(instanceID));
}

float3 LocalToNDCDirOnly(float3 localDir, uint instanceID)//로컬 포지션을 월드로
{
    float3 dir_world = LocalToWorldDirOnly(localDir, instanceID);
  
    return WorldToNDCDirOnly(dir_world);
}


















/////버텍스용
//float3 WorldToLocal(float3 localPos)//월드 포지션을 월드로
//{
//	return mul(world, float4(localPos, 1.0f));
//}

//float4 NDCToWorld(float4 worldPos)
//{
//    float4 ret = mul(view, worldPos);
//    ret = mul(proj, ret);
    
//    return ret;
//}
//float4 NDCToLocal(float4 localPos)//로컬 포지션을 월드로
//{
//    float4 ret = LocalToWorld(localPos);
//    ret = WorldToNDC(ret);
    
//    return ret;
//}

///쓰지 말것 구현한 것 아님, 
/////벡터용
//float3 WorldToLocalDirOnly(float3 localDir)//로컬 포지션을 월드로
//{
//    return mul((float3x3) world, localDir);
//}

//float3 NDCToWorldDirOnly(float3 worldDir)
//{
//    float3 ret = mul((float3x3) view, worldDir);
//    ret = mul((float3x3) proj, ret);
    
//    return ret;
//}

//float3 NDCToLocalDirOnly(float3 localDir)//로컬 포지션을 월드로
//{
//    float3 ret = LocalToWorldDirOnly(localDir);
//    ret = WorldToNDCDirOnly(ret);
    
//    return ret;
//}

#endif