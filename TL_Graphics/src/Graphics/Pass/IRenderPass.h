#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "ShaderResourceSlotPS.h"

#include "Texture.h"

#include "RenderTarget.h"

#include "RenderTargetTexture.h"

#include "OnResize.h"

class Pipeline;

class IRenderPass : public OnResize_Observer
{
public:
	IRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT rttNum, UINT sourceTextureNum);
	~IRenderPass();

	virtual void Set() = 0;

	virtual void Execute() {};

	void ClearRenderTargets(TL_Math::Vector4 color = { 0,0,0,0 });

	//자기 자신의 텍스쳐는 꽂으면 안됨 ex) t->SetSourceTexture(t->GetDestTexture); 추후 UAV공부해서 계선 할 수 있을 듯?
	void SetSourceTexture(Texture* texture, UINT sourceTextureNum = 0);

	void CreateDestTexture(UINT renderTargetNum = 0, string debugName = "", DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
	void DeleteDestTextures();
	void SetDestTexture(RenderTargetTexture* renderTarget, UINT renderTargetNum = 0);
	RenderTargetTexture* GetDestTexture(UINT destTextureNum = 0);

	//둘 중 하나만 동작함, Ratio는 화면 비율, Size = 픽셀 수
	void SetRatio(float sizeX, float sizeY);//(0.0, 1.0]
	void SetSize(UINT sizeX, UINT sizeY);

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;
	OnResizeNotice* resizeNotice;

	vector<RenderTargetTexture*> rtts;
	vector<Texture*> sourceTextures;

	//rtts Size
	bool isBasedWindowSize = true;
	float widthRatio = 1.0f, heightRatio = 1.0f;
	UINT width = 0, height = 0;
	D3D11_VIEWPORT viewPort;

	void DescViewPort();
	void ResizeViewport();
	void ResizeRTTs();

	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;

};