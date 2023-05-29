#include "pch_dx_11.h"
#include "LightPass.h"

#include "Pipeline.h"

LightPass::LightPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	, resizeNotice(resizeNotice)
{
	//CreateRenderTarget(resizeNotice);
	CreateShader();
}

LightPass::~LightPass()
{
	SAFE_DELETE(shaderPS);
}

void LightPass::Set()
{
	rtts[0]->SetRT(0);
	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
	shaderPS->Set();
}

void LightPass::Execute()
{
	ClearRenderTargets();

	Set();

	pipeline->Draw();
}


void LightPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	/*rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "BloomCurve");*/
}

void LightPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_LightPass.hlsl");
}
