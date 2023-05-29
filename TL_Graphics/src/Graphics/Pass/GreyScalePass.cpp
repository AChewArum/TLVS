#include "pch_dx_11.h"
#include "GreyScalePass.h"

#include "Pipeline.h"

GreyScalePass::GreyScalePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
	CreateShader();
}

GreyScalePass::~GreyScalePass()
{
}

void GreyScalePass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();
	assert(sourceTextures[0]);
	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}

void GreyScalePass::Execute()
{
	auto oldViewport = pipeline->SetViewPort(&viewPort);

	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}


void GreyScalePass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_Luminance.hlsl");
}
