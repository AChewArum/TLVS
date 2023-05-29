#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    float4 ret = float4(0, 0, 0, 0);
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            // Compute the sum of color values
            ret += sourceSlot0.Sample(Sampler_Clamp, surface.uv, int2(x, y));
        }
    }
    
    // Divide the sum to complete the average
    ret /= 9;
    
    return float4(ret.rgb, 1.0f);
}