#include "pch_dx_11.h"
#include "LightAdaptionPass.h"

#include "Pipeline.h"

LightAdaptionPass::LightAdaptionPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 2)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();

}

LightAdaptionPass::~LightAdaptionPass()
{
	SAFE_DELETE(shaderPS);
}

void LightAdaptionPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);//original
	sourceTextures[1]->Set(TL_Graphics::E_SHADER_TYPE::PS, source1Slot);//Luminanace
}

void LightAdaptionPass::Execute()
{
	ClearRenderTargets();

	auto oldViewport = pipeline->SetViewPort(&viewPort);

	Set();

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void LightAdaptionPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
}

void LightAdaptionPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_LightAdaptionPass.hlsl");
}
