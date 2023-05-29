SamplerState Sampler : register(s0);

Texture2D pbr : register(t0);
Texture2D legacy : register(t1);

struct VSout
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 main(VSout input) : SV_Target0
{
    float4 ret = float4(1, 0, 1, 1);
    
    ////////////////////////////////
    
    if (input.uv.x <= 0.501f && input.uv.x >= 0.499f)
        ret = float4(0, 0, 0, 1);
    else if(input.uv.x > 0.501f)
        ret = pbr.Sample(Sampler, input.uv);
    else
        ret = legacy.Sample(Sampler, input.uv);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    return ret;
}