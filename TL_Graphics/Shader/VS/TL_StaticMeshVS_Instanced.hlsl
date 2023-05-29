#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out main(VS_In_StaticMesh v,
    uint instanceID : SV_InstanceID)
{
    VS_Out ret;
    
    ret.pos_world = LocalToWorld(v.pos_local);
    ret.pos_view = WorldToView(ret.pos_world);
    ret.pos_NDC = ViewToNDC(ret.pos_view);
    
    ret.uv = v.uv;

    ret.normal_world = LocalToWorldDirOnly(v.normal_local, instanceID);
    ret.tangent_world = LocalToWorldDirOnly(v.tangent_local, instanceID);
    ret.bitangent_world = LocalToWorldDirOnly(v.bitangent_local, instanceID);
    
    
    ret.instanceID = instanceID;
    
    return ret;
}