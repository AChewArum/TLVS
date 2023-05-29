#ifndef TL_Reinhard
#define TL_Reinhard

/// functions for ToneMapping

float Luminance(float3 v)//color의 밝기를 구해준다. 사람은 초록색을 파란색보다 밝게 인지한다.
{
    return max(0.001f, dot(v, float3(0.2126f, 0.7152f, 0.0722f)));
}

float3 Change_luminance(float3 c_in, float l_out)//칼라의 밝기를 바꿔준다.
{
    float l_in = Luminance(c_in);
    return c_in * (l_out / l_in);
}

float Reinhard_Luminance(float luminance, float maxWhite)
{
    float numerator = luminance * (1.0f + (luminance / pow(maxWhite, 2.0f)));
    
    return numerator / (1.0f + luminance);
}

float3 Reinhard_Color(float3 color, float maxWhite)
{
    float3 numerator = color * (1.0f + (color / pow(maxWhite, 2.0f)));
    
    return numerator / (1.0f + color);
}

#endif