

struct VSin
{
    float3 pos : POSITION;
    float2 uv : UV;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSout main(VSin input)
{
    VSout output;
    
    output.pos = float4(input.pos, 1);
    output.uv = input.uv;
    
    return output;
}
