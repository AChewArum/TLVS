#ifndef TL_IO_VS
#define TL_IO_VS

struct VS_In_StaticMesh
{
	float3 pos_local : POSITION;
    
	float2 uv : UV;
    
	float3 normal_local : NORMAL;
	float3 tangent_local : TANGENT;
	float3 bitangent_local : BITANGENT;
};

struct VS_In_SkinningMesh
{
	float3 pos_local : POSITION;
    
	float2 uv : UV;
    
	float3 normal_local : NORMAL;
	float3 tangent_local : TANGENT;
	float3 bitangent_local : BITANGENT;
    
	uint4 boneIndex : BONE_INDEX;
	float4 boneWeight : BONE_WEIGHT;
};

struct VS_Out//aka PS_In
{
	float3 pos_world : POSITION_W;
    float3 pos_view : POSITION_V;
	float4 pos_NDC : SV_POSITION;
    float linearDepth : LINEAR_DEPTH;//0.0~1.0 depth linear
    
	float2 uv : UV;
    
	float3 normal_world : NORMAL;
	float3 tangent_world : TANGENT;
	float3 bitangent_world : BITANGENT;
	
    uint instanceID : SV_InstanceID;
};

struct VS_In_ScreenSpace
{
	float2 pos : POSITION;
    
	float2 uv : UV;
};

struct VS_Out_ScreenSpace//aka PS_In
{
	float4 pos_NDC : SV_POSITION;
    
    float2 uv : UV;
    uint instanceID : SV_InstanceID;
};

struct VS_In_Pos
{
	float3 pos_local : POSITION;
};

struct VS_Out_SkyBox
{
	float4 pos_NDC : SV_POSITION;
    float3 uvw : UVW;
};

#endif