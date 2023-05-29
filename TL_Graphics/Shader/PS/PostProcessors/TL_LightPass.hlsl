#include "TL_ConstantsPS.hlsli"

#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

//#define BLOOMCURVE_METHOD_1
//#define BLOOMCURVE_METHOD_2
#define BLOOMCURVE_METHOD_3

float GetBloomCurve(float x, float threshold)
{
    float result = x;
    x *= 2.0f;
    
#ifdef BLOOMCURVE_METHOD_1
    result = x*0.05+max(0,x-threshold)*0.5; // default threshold = 1.26
#endif
    
#ifdef BLOOMCURVE_METHOD_2
    result = x*x/3.2;
#endif
    
#ifdef BLOOMCURVE_METHOD_3
    result = max(0, x - threshold); // default threshold = 1.0
    result *= result;
#endif 
    
    return result * 0.5f;
}

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    float4 rgba = sourceSlot0.Sample(Sampler_Clamp, surface.uv);
	
    float intensity = dot(rgba.rgb, float3(0.3f, 0.3f, 0.3f));
    float bloom_intensity = GetBloomCurve(intensity, 1.0f);
    
    
    return float4(float3(rgba.rgb * bloom_intensity / intensity),1.0f);
}