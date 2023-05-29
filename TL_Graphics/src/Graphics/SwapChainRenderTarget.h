#pragma once

#include "OnResize.h"

#include "RenderTarget.h"

class SwapChainRenderTarget : public RenderTarget, public OnResize_Observer
{
public:
	SwapChainRenderTarget(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~SwapChainRenderTarget();

	void Init(uint32_t width, uint32_t height);

	virtual ID3D11RenderTargetView* Set(UINT slot = 0) override;


	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;

};