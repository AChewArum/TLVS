#include "pch_dx_11.h"
#include "RenderTarget.h"

#include "Pipeline.h"


RenderTarget::RenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::string debugName)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{
	if (debugName.length() > 0)
		rtv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
}

RenderTarget::~RenderTarget()
{
}

ID3D11RenderTargetView* RenderTarget::Set(UINT slot)
{
	return pipeline->SetRenderTarget(rtv, slot);
}

void RenderTarget::SetOnce(UINT slot)
{
	pipeline->SetRenderTargetOnce(rtv, slot);
}

void RenderTarget::Clear(TL_Math::Vector4 color)
{
	pipeline->ClearRenderTarget(rtv, color);
}
