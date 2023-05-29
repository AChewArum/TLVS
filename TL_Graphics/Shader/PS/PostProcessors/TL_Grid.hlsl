#include "TL_IO_VS.hlsli"

#include "TL_ConstantsPS.hlsli"


//화면 공간에서 z = 0 일때 그리드를 그려주는 쉐이더


//1차함수만 쓸 것
bool CloseToEqual(float a, float b, float gap)//gap == 선의 굵기
{
	return abs(a - b) < gap;
}

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
	
	float NDC_x = surface.uv.x * 2.0f - 1.0f;
	float NDC_y = -(surface.uv.y * 2.0f - 1.0f);
    
	float3 NDC_xyz = float3(NDC_x, NDC_y, 0.0f);
	
	float3 rayOrigin_ViewSpace = mul(cam.projInv, float4(NDC_xyz, 1.0f)).xyz;
    
	float3 rayOrigin_World = mul(cam.viewInv, float4(rayOrigin_ViewSpace, 1.0f)).
	xyz;
	
	float3 rayDirection = rayOrigin_World - cam.camPos;
	
	if (rayDirection.y * cam.camPos.y > 0) 
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
    
	
	float t = -rayOrigin_World.y / rayDirection.y; //거리
    
    
	float alpha = (500 - t) / 500.0f;
	
	float3 intersection = rayOrigin_World + t * rayDirection;
    
    
	if (CloseToEqual(intersection.x, 0.0f, 0.5f))
		return float4(0, 0, 1, 1);
    
	if (CloseToEqual(intersection.z, 0.0f, 0.5f))
		return float4(1, 0, 0, 1);
    
	
	
	if (CloseToEqual(intersection.x % 100, 0.0f, 0.3f))
		return float4(1, 1, 1, alpha);
    
	
	if (CloseToEqual(intersection.z % 100, 0.0f, 0.3f))
		return float4(1, 1, 1, alpha);
    
	
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}