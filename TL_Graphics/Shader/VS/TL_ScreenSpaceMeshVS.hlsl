#include "TL_IO_VS.hlsli"

VS_Out_ScreenSpace main(VS_In_ScreenSpace v)
{
    VS_Out_ScreenSpace ret;
	
    ret.pos_NDC = float4(v.pos.x, v.pos.y, float2(0.0f, 1.0f));
    ret.uv = v.uv;
	
	return ret;
}