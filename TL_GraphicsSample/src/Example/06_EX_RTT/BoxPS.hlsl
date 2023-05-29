cbuffer Color : register(b0)
{
    float4 color;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 posW : WORLD_POS;
};

struct PSout
{
    float4 out0 : SV_Target0;
    float4 out1 : SV_Target1;
    float4 out2 : SV_Target2;
    float4 out3 : SV_Target3;
};

//MRT ¿¹Á¦ »Ì´Â Áß

PSout main(VSout input)
{
    PSout result;
    
    result.out0 = color;//albedo
    
    result.out1 = float4(input.normal, 1.0f);
    
    result.out2 = float4(input.posW, 1.0f);
    
    result.out3 = float4(input.pos.z, input.pos.z, input.pos.z, 1);
    
    return result;
}