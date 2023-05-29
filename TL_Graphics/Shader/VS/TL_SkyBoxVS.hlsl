#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out_SkyBox main(VS_In_Pos v)
{
	VS_Out_SkyBox ret;
	
	float3 worldPos = camPos + v.pos_local;
	
	ret.pos_NDC = WorldToNDC(worldPos);
	//ret.pos_NDC = LocalToNDC(v.pos_local);
    
	ret.uvw = v.pos_local;
	
	return ret;
}