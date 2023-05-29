#include "pch_dx_11.h"
#include "DeferredRenderPass.h"

#include "Pipeline.h"

DeferredRenderPass::DeferredRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice,1,8)
{
	//CreateRenderTarget(resizeNotice);
	CreateShader();
}

DeferredRenderPass::~DeferredRenderPass()
{
	//SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void DeferredRenderPass::Set()
{
	rtts[0]->SetRT(0);
	shaderPS->Set();

	for (UINT i = 0; i < 8; i++)
		sourceTextures[i]->Set(TL_Graphics::E_SHADER_TYPE::PS, albedo_DeferredSlot + i);

}

void DeferredRenderPass::Execute()
{
	//ClearRenderTargets();
	Set();
	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	rtts[0]->SetT(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}


void DeferredRenderPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	//rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "Deferred");
}

void DeferredRenderPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DeferredPS.hlsl", "Deferred");
}
