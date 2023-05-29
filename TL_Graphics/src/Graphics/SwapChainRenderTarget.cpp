#include "pch_dx_11.h"
#include "SwapChainRenderTarget.h"

#include "Pipeline.h"

SwapChainRenderTarget::SwapChainRenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:RenderTarget(dc, resources, pipeline)
{
	resizeNotice->AddObserver(this);
}

SwapChainRenderTarget::~SwapChainRenderTarget()
{
}

void SwapChainRenderTarget::Init(uint32_t width, uint32_t height)
{
	OnResize(width, height);
}

ID3D11RenderTargetView* SwapChainRenderTarget::Set(UINT slot)
{
	return pipeline->SetRenderTarget(rtv, slot);
}

void SwapChainRenderTarget::OnResize(uint32_t width, uint32_t height)
{
	HRESULT hr = S_OK;

	rtv.Return();

	ID3D11Texture2D* backBuffer = nullptr;
	pipeline->swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	hr = pipeline->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	resources->rtvs->CreateDefault(rtv, backBuffer);

	backBuffer->Release();
	backBuffer = nullptr;

	const char* temp = "SwapChainRTV";
		rtv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(temp), temp);


	assert(SUCCEEDED(hr));

	//todo : 일단 이렇게 해놓긴 하는데, 바꿔야한다. swapChainRTV Set 시점 고민해봐야함, postProcssing 거치면
	Set();
}
