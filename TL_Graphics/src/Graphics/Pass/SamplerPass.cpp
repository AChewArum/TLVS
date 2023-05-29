#include "pch_dx_11.h"
#include "SamplerPass.h"

#include "Pipeline.h"

SamplerPass::SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, Shader* shaderPS)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
	, shaderPS(shaderPS)
{
	//CreateRenderTarget(resizeNotice);

	if (!shaderPS)
		CreateShader();

	sourceTextures.resize(1);

	DescViewPort();
	ResizeViewport();
}

SamplerPass::~SamplerPass()
{
	//SAFE_DELETE(rtts[0]);

	if(isShaderCreatedHere)//스마트 포인터 공부하고 고칠 것
	SAFE_DELETE(shaderPS);
}

void SamplerPass::Set()
{
	//pipeline->SetSwapChainRenderTargetView(0);
	rtts[0]->SetRT(0);
	shaderPS->Set();

	assert(sourceTextures[0]);
	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}

void SamplerPass::Execute()
{
	//ClearRenderTargets();

	auto oldViewport = pipeline->SetViewPort(&viewPort);

	Set();

	pipeline->Draw();

	//정리
	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void SamplerPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	//if (isBasedWindowSize)
	//	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, widthRatio, heightRatio);
	//else //!isBasedWindowSize
	//	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, width, height);
}

void SamplerPass::CreateShader()
{
	isShaderCreatedHere = true;
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_SamplerPS.hlsl");
}
