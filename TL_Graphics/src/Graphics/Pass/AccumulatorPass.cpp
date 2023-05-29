#include "pch_dx_11.h"
#include "AccumulatorPass.h"

#include "Pipeline.h"

AccumulatorPass::AccumulatorPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 2)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();
}

AccumulatorPass::~AccumulatorPass()
{
	SAFE_DELETE(shaderPS);
}

void AccumulatorPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
	sourceTextures[1]->Set(TL_Graphics::E_SHADER_TYPE::PS, source1Slot);
}

void AccumulatorPass::Execute()
{
	Set();
	auto oldViewport = pipeline->SetViewPort(&viewPort);

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}

void AccumulatorPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	/*rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "Accumulator");*/
}

void AccumulatorPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/Accumulator.hlsl");
}
