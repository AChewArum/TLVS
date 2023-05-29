#ifndef TL_Samplers
#define TL_Samplers

SamplerState Sampler_Wrap : register(s0);
SamplerState Sampler_Mirror : register(s1);
SamplerState Sampler_Clamp : register(s2);
SamplerState Sampler_Border : register(s3);

SamplerComparisonState Sampler_Comp : register(s4);//Shadow
#endif