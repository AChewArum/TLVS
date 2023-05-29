#include "pch_dx_11.h"
#include "RenderTargetTexture.h"

#include "Pipeline.h"

RenderTargetTexture::RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height, std::string debugName, DXGI_FORMAT format)
	:Texture(dc, resources, pipeline), RenderTarget(dc, resources, pipeline)
	, isBasedWindowSize(false)
	, dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, format(format)
	, resizeNotice(resizeNotice)
{
	sizeX = width;
	sizeY = height;

	ResizeRTT(sizeX, sizeY);

	SetDebugName(debugName);
}

RenderTargetTexture::RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio, float heightRatio, std::string debugName, DXGI_FORMAT format)
	: Texture(dc, resources, pipeline), RenderTarget(dc, resources, pipeline)
	, widthRatio(widthRatio)
	, heightRatio(heightRatio)
	, isBasedWindowSize(true)
	, dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, format(format)
	, resizeNotice(resizeNotice)
{
	resizeNotice->AddObserver(this);

	sizeX = resizeNotice->GetWidth() * widthRatio < 1 ? 1 : resizeNotice->GetWidth() * widthRatio;
	sizeY = resizeNotice->GetHeight() * heightRatio < 1 ? 1 : resizeNotice->GetHeight();

	ResizeRTT(sizeX, sizeY);

	SetDebugName(debugName);
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::SetRT(UINT slot)
{
	RenderTarget::Set(slot);
}

ID3D11RenderTargetView* RenderTargetTexture::SetRTTEST(UINT slot)
{
	return RenderTarget::Set(slot);
}

void RenderTargetTexture::SetRTOnce(UINT slot)
{
	RenderTarget::SetOnce(slot);
}

void RenderTargetTexture::SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	Texture::Set(type, slot);
}

void RenderTargetTexture::Clear(TL_Math::Vector4 color)
{
	pipeline->ClearRenderTarget(rtv, color);
}

void RenderTargetTexture::OnResize(uint32_t width, uint32_t height)
{
	sizeX = width * widthRatio < 1 ? 1 : width * widthRatio;
	sizeY = height * heightRatio < 1 ? 1 : height * heightRatio;

	ResizeRTT(sizeX, sizeY);
}

void RenderTargetTexture::ResizeRTT(UINT sizeX, UINT sizeY)
{//
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = sizeX;
	desc.Height = sizeY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	resources->texture2Ds->Create(texture, desc);

	//
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	resources->rtvs->Create(rtv, rtvDesc, texture);

	//
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	resources->srvs->Create(srv, srvDesc, texture);
}

void RenderTargetTexture::Resize(UINT sizeX, UINT sizeY)
{
	isBasedWindowSize = false;

	ResizeRTT(sizeX, sizeY);
}

void RenderTargetTexture::ResizeRatio(float widthRatio, float heightRatio)
{
	isBasedWindowSize = true;

	this->widthRatio = widthRatio;
	this->heightRatio = heightRatio;

	OnResize(resizeNotice->GetWidth(), resizeNotice->GetHeight());
}

void RenderTargetTexture::SetDebugName(std::string debugName)
{
	if (debugName.length() > 0)
	{
		string temp = debugName + "Texture";
		texture.resource->SetPrivateData(WKPDID_D3DDebugObjectName, temp.length(), temp.c_str());

		temp = debugName + "RTV";
		rtv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, temp.length(), temp.c_str());

		temp = debugName + "SRV";
		srv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, temp.length(), temp.c_str());
	}
}
