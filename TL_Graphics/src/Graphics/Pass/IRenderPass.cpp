#include "pch_dx_11.h"
#include "IRenderPass.h"

IRenderPass::IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT rttNum, UINT sourceTextureNum)
	: dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, resizeNotice(resizeNotice)
{
	resizeNotice->AddObserver(this);
	rtts.resize(rttNum);
	sourceTextures.resize(sourceTextureNum);
	DescViewPort();
	ResizeViewport();
}

IRenderPass::~IRenderPass()
{
}

void IRenderPass::ClearRenderTargets(TL_Math::Vector4 color)
{
	for (auto rtt : rtts)
		rtt->Clear(color);
}

void IRenderPass::SetSourceTexture(Texture* texture, UINT sourceTextureNum)
{
	sourceTextures[sourceTextureNum] = texture;
}

void IRenderPass::CreateDestTexture(UINT renderTargetNum, string debugName, DXGI_FORMAT format)
{
	rtts[renderTargetNum] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, debugName, format);
}

void IRenderPass::DeleteDestTextures()
{
	for (auto rtt : rtts)
		delete rtt;
}

void IRenderPass::SetDestTexture(RenderTargetTexture* renderTarget, UINT renderTargetNum)
{
	rtts[renderTargetNum] = renderTarget;
}

RenderTargetTexture* IRenderPass::GetDestTexture(UINT destTextureNum)
{
	return rtts[destTextureNum];
}

void IRenderPass::SetRatio(float sizeX, float sizeY)
{
	isBasedWindowSize = true;
	widthRatio = sizeX;
	heightRatio = sizeY;


	ResizeViewport();
	ResizeRTTs();
}

void IRenderPass::SetSize(UINT sizeX, UINT sizeY)
{
	isBasedWindowSize = false;
	width = sizeX;
	height = sizeY;

	ResizeViewport();
	ResizeRTTs();
}

void IRenderPass::DescViewPort()
{
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void IRenderPass::ResizeViewport()
{
	if (isBasedWindowSize)
	{
		assert(widthRatio > 0.0f);//assert When value is not set
		assert(heightRatio > 0.0f);

		viewPort.Width = resizeNotice->GetWidth() * widthRatio;
		viewPort.Height = resizeNotice->GetHeight() * heightRatio;
	}
	else
	{
		assert(width > 0);
		assert(height > 0);

		viewPort.Width = (float)width;
		viewPort.Height = (float)height;
	}
}

void IRenderPass::ResizeRTTs()
{
	if (isBasedWindowSize)
	{
		assert(widthRatio > 0.0f);//assert When value is not set
		assert(heightRatio > 0.0f);

		for (auto rtt : rtts)
		{
			if(rtt)
			rtt->ResizeRatio(widthRatio, heightRatio);
		}
	}
	else
	{
		assert(width > 0);
		assert(height > 0);

		for (auto rtt : rtts)
		{
			if (rtt)
			rtt->Resize(width, height);
		}
	}
}

void IRenderPass::OnResize(uint32_t width, uint32_t height)
{
	ResizeViewport();
	ResizeRTTs();
}
