SamplerState Sampler : register(s0);

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D posWMap : register(t2);
Texture2D depthMap : register(t3);
struct VSout
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 main(VSout input) : SV_Target0
{
    float4 ret = float4(1, 0, 1, 1);
    
    ////////////////////////////////
    
    if(input.uv.y < 0.499f)
    {
        if (input.uv.x > 0.501f)
            ret = albedoMap.Sample(Sampler, input.uv);
        else
            ret = normalMap.Sample(Sampler, input.uv);
        
    }
    if(input.uv.y > 0.501f)
    {
        if (input.uv.x > 0.501f)
            ret = posWMap.Sample(Sampler, input.uv);
        else
            ret = depthMap.Sample(Sampler, input.uv);
        
    }
    
    if (input.uv.x <= 0.501f && input.uv.x >= 0.499f)
        ret = float4(0, 0, 0, 1);
    if (input.uv.y <= 0.501f && input.uv.y >= 0.499f)
        ret = float4(0, 0, 0, 1);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    
    return ret;
}