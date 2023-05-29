#include "pch_dx_11.h"
#include "GaussianBlurPass.h"

#include "Pipeline.h"

GaussianBlurPass::GaussianBlurPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, TL_Math::Vector2 direction)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 1)
	
{
	dir_info.dir = direction;


	dirInfoBuffer = new ConstantBuffer(dc, resources, pipeline, &dir_info, sizeof(Dir_Info));

	//CreateRenderTarget(resizeNotice);
	CreateShader();
}

GaussianBlurPass::~GaussianBlurPass()
{
	SAFE_DELETE(dirInfoBuffer);

	//SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void GaussianBlurPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	sourceTextures[0]->Set(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);

	dirInfoBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
}

void GaussianBlurPass::Execute()
{
	//ClearRenderTargets();
	Set();

	auto oldViewport = pipeline->SetViewPort(&viewPort);

	pipeline->Draw();

	//rtts[0]->SetT(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	pipeline->SetViewPort(oldViewport);
}
//
//void GaussianBlurPass::ClearRenderTargets()
//{
//	rtts[0]->Clear();
//}

void GaussianBlurPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	//rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "GaussianBlur");
}

void GaussianBlurPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/GaussianBlur.hlsl");
}
