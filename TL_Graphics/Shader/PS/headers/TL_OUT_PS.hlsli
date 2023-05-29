#ifndef TL_OUT_PS
#define TL_OUT_PS

struct PS_Out
{
    float4 out0 : SV_Target0;//albedo
    float4 out1 : SV_Target1;//normal_world
    float4 out2 : SV_Target2;//pos_world
    float4 out3 : SV_Target3;//metalness
    float4 out4 : SV_Target4;//roughness
    float4 out5 : SV_Target5;
    float4 out6 : SV_Target6;
    float4 out7 : SV_Target7;
};

#endif