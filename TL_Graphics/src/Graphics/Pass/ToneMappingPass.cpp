#include "pch_dx_11.h"
#include "ToneMappingPass.h"

#include "Pipeline.h"

ToneMappingPass::ToneMappingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
	CreateShader();
}

ToneMappingPass::~ToneMappingPass()
{
	SAFE_DELETE(shaderPS);
}

void ToneMappingPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}

void ToneMappingPass::Execute()
{
	Set();
	auto oldViewport = pipeline->SetViewPort(&viewPort);

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void ToneMappingPass::CreateRenderTargets(OnResizeNotice* resizeNotice)
{
}

void ToneMappingPass::DeleteRenderTargets()
{
}

void ToneMappingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ToneMapping.hlsl");
}
