#pragma once

#include "IRenderTargetTexture.h"

#include "ShaderResource.h"
#include "RenderTarget.h"
#include "OnResize.h"

#include "Texture.h"
/// <summary>
/// 
/// 
/// 주의점 : SetRT로 그린 텍스쳐를 사용하기(SetT) 전에 RenderTarget을 UnSet해야한다.(pipeline->UnSetRenderTarget
/// </summary>

class RenderTargetTexture : public TL_Graphics::IRenderTargetTexture,  public Texture, public OnResize_Observer,  public RenderTarget
{
public:
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, UINT width, UINT height, std::string debugName = "", DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
	RenderTargetTexture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, float widthRatio = 1.0f, float heightRatio = 1.0f, std::string debugName = "", DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
	virtual ~RenderTargetTexture();

	// IRenderTargetTexture을(를) 통해 상속됨

	virtual void SetRT(UINT slot) override;
	ID3D11RenderTargetView* SetRTTEST(UINT slot) ;//todo : 일단 돌아가게만 하자.. 나중에 정리할 것
	//ID3D11RenderTargetView* SetRT(UINT slot)

	void SetRTOnce(UINT slot) ;

	virtual void SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot) override;

	virtual void Clear(TL_Math::Vector4 color = { 0,0,0,0 }) override;

	void Resize(UINT sizeX, UINT sizeY);
	void ResizeRatio(float widthRatio, float heightRatio);
private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;
	OnResizeNotice* resizeNotice;

	Resource<ID3D11Texture2D> texture;


	bool isBasedWindowSize = true;
	float widthRatio, heightRatio;
	// OnResize_Observer을(를) 통해 상속됨
	virtual void OnResize(uint32_t width, uint32_t height) override;
	void ResizeRTT(UINT sizeX, UINT sizeY);

	bool initialized = false;

	DXGI_FORMAT format;

	void SetDebugName(std::string debugName);
};