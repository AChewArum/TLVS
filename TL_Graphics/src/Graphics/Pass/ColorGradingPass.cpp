#include "pch_dx_11.h"
#include "ColorGradingPass.h"

#include "Pipeline.h"

ColorGradingPass::ColorGradingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice,1,1)
	, resizeNotice(resizeNotice)
{
	CreateRenderTarget(resizeNotice);
	CreateShader();


	LUT = new Texture(dc, resources, pipeline, L"_DevelopmentAssets/Texture/volumeTexture_Greenish.dds", "ColorGradingLUT");
}

ColorGradingPass::~ColorGradingPass()
{
	SAFE_DELETE(LUT);

	SAFE_DELETE(rtts[0]);
	SAFE_DELETE(shaderPS);
}

void ColorGradingPass::Set()
{
	LUT->Set(TL_Graphics::E_SHADER_TYPE::PS, LUTSlot);

	rtts[0]->SetRT(0);
	shaderPS->Set();
}

void ColorGradingPass::Execute()
{
	ClearRenderTargets();

	Set();
	pipeline->Draw();
	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
	rtts[0]->SetT(TL_Graphics::E_SHADER_TYPE::PS, source0Slot);
}


void ColorGradingPass::CreateRenderTarget(OnResizeNotice* resizeNotice)
{
	rtts[0] = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, 1.0f, 1.0f, "ColorGrading");
}

void ColorGradingPass::CreateShader()
{
	shaderPS = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_ColorGrading.hlsl", "ColorGradingPS");
}
