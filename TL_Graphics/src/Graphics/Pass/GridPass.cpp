#include "pch_dx_11.h"
#include "GridPass.h"

#include "Pipeline.h"

GridPass::GridPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice, 1, 0)
{
	CreateShader();
}

GridPass::~GridPass()
{
	//SAFE_DELETE(rtt);
	SAFE_DELETE(shaderPS);
}

void GridPass::Set()
{
	pipeline->SetSwapChainRenderTargetView(0);
	shaderPS->Set();
}

void GridPass::Execute()
{
	Set();
	pipeline->Draw();
}

void GridPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
}

void GridPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_Grid.hlsl", "Grid");
}
