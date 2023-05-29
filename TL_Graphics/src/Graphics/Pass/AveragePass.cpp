#include "pch_dx_11.h"
#include "AveragePass.h"

#include "Pipeline.h"

AveragePass::AveragePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
	CreateShader();
}

AveragePass::~AveragePass()
{
}

void AveragePass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();
	assert(sourceTextures[0]);
	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}

void AveragePass::Execute()
{
	auto oldViewport = pipeline->SetViewPort(&viewPort);

	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void AveragePass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_3x3Average.hlsl");
}
