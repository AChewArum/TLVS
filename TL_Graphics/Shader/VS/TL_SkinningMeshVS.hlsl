#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out main(VS_In_SkinningMesh v)
{
    VS_Out ret;

    // skin

    float3 posW = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);

    float totalWeight = 0;
    
    for (int i = 0; i < 4; i++)
    {
        int boneIndex = v.boneIndex[i];
        float boneWeight = v.boneWeight[i];

        if (boneIndex > -1 && boneIndex < 128)
        {
            totalWeight += boneWeight;
            
            posW += boneWeight * mul(g_BoneTransforms[boneIndex], float4(v.pos_local, 1.0f)).xyz;
            normalL += boneWeight * mul((float3x3) g_BoneTransforms[boneIndex], v.normal_local).xyz;
            tangentL += boneWeight * mul((float3x3) g_BoneTransforms[boneIndex], v.tangent_local).xyz;
        }
    }

    // TODO: normalize랑 무엇이 다르지..?
    posW /= totalWeight;
    normalL /= totalWeight;
    tangentL /= totalWeight;

    ret.pos_world = posW;
    ret.pos_view = WorldToView(ret.pos_world);
    ret.linearDepth = ret.pos_view.z / frustumFar;
    ret.pos_NDC = ViewToNDC(ret.pos_view);

    ret.uv = v.uv;

    ret.normal_world = normalize(normalL);
    ret.tangent_world = normalize(tangentL);
    ret.bitangent_world = LocalToWorldDirOnly(tangentL);

    // end skin

    // ret.pos_world = LocalToWorld(float4(v.pos_local, 1.0f));
    // ret.pos_NDC = WorldToNDC(float4(ret.pos_world, 1.0f));
    //
    // ret.uv = v.uv;
    //
    // ret.normal_world = LocalToWorldDirOnly(v.normal_local);
    // ret.tangent_world = LocalToWorldDirOnly(v.tangent_local);
    // ret.bitangent_world = LocalToWorldDirOnly(v.bitangent_local);
    
    return ret;
}