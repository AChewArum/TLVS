#include "pch_dx_11.h"
#include "BloomPass.h"

#include "Pipeline.h"

BloomPass::BloomPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:IRenderPass(dc, resources, pipeline, resizeNotice,1,1)
	, resizeNotice(resizeNotice)
{

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 1,0 });
	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 0,1 });

	accumulatorPass = new AccumulatorPass(dc, resources, pipeline, resizeNotice);
	accumulatorPass0 = new AccumulatorPass(dc, resources, pipeline, resizeNotice);

	samplerPass = new SamplerPass(dc, resources, pipeline, resizeNotice);

	lightPass = new LightPass(dc, resources, pipeline, resizeNotice);

	CreateRenderTargets(resizeNotice);
	CreateShader();
}

BloomPass::~BloomPass()
{
	DeleteRenderTargets();

	SAFE_DELETE(lightPass);
	SAFE_DELETE(samplerPass);
	//SAFE_DELETE(samplerPass0);
	SAFE_DELETE(accumulatorPass);
	SAFE_DELETE(accumulatorPass0);
	SAFE_DELETE(gaussianBlurPassX);
	SAFE_DELETE(gaussianBlurPassY);
}

void BloomPass::Set()
{
}

void BloomPass::Execute()
{
	ClearRenderTargets();

	//밝은 부분을 추출하고(LightPass) LightRT

	//DownSampling을 실시한다. 약 3단계로
	lightPass->SetSourceTexture(sourceTextures[0]);
	lightPass->Execute();
	Texture* lightmap = lightPass->GetDestTexture();

	//가장 낮은 해상도의 RT를 Blur한다.
	gaussianBlurPassX->SetRatio(0.25f, 0.25f);
	gaussianBlurPassX->SetSourceTexture(lightmap);
	gaussianBlurPassX->Execute();

	gaussianBlurPassY->SetRatio(0.25f, 0.25f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();


	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(0.5f, 0.5f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(0.5f, 0.5f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(),0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(),1);
	accumulatorPass->Execute();

	gaussianBlurPassX->ClearRenderTargets();
	samplerPass->ClearRenderTargets();
	gaussianBlurPassY->ClearRenderTargets();

	//그 텍스쳐를 Blur한다.
	gaussianBlurPassX->SetRatio(0.5f, 0.5f);
	gaussianBlurPassX->SetSourceTexture(accumulatorPass->GetDestTexture());
	gaussianBlurPassX->Execute();

	accumulatorPass->ClearRenderTargets();

	gaussianBlurPassY->SetRatio(0.5f, 0.5f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();

	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(0.75f, 0.75f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(0.75f, 0.75f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(),0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(),1);
	accumulatorPass->Execute();

	gaussianBlurPassX->ClearRenderTargets();
	samplerPass->ClearRenderTargets();
	gaussianBlurPassY->ClearRenderTargets();


	//그 텍스쳐를 Blur한다.
	gaussianBlurPassX->SetRatio(0.75f, 0.75f);
	gaussianBlurPassX->SetSourceTexture(accumulatorPass->GetDestTexture());
	gaussianBlurPassX->Execute();

	accumulatorPass->ClearRenderTargets();

	gaussianBlurPassY->SetRatio(0.75f, 0.75f);
	gaussianBlurPassY->SetSourceTexture(gaussianBlurPassX->GetDestTexture());
	gaussianBlurPassY->Execute();

	//그 위의 단계의 RT와 Accumulate한다.
	samplerPass->SetRatio(1.0f, 1.0f);
	samplerPass->SetSourceTexture(lightmap);
	samplerPass->Execute();
	accumulatorPass->SetRatio(1.0f, 1.0f);
	accumulatorPass->SetSourceTexture(gaussianBlurPassY->GetDestTexture(), 0);
	accumulatorPass->SetSourceTexture(samplerPass->GetDestTexture(), 1);
	accumulatorPass->Execute();

	gaussianBlurPassX->ClearRenderTargets();
	samplerPass->ClearRenderTargets();
	gaussianBlurPassY->ClearRenderTargets();

	//마지막으로 원본과 합쳐준다.
	accumulatorPass0->SetDestTexture(rtts[0]);
	accumulatorPass0->SetRatio(1.0f, 1.0f);
	accumulatorPass0->SetSourceTexture(accumulatorPass->GetDestTexture(), 0);
	accumulatorPass0->SetSourceTexture(sourceTextures[0], 1);
	accumulatorPass0->Execute();

	accumulatorPass->ClearRenderTargets();

	pipeline->UnSetRenderTarget(0);
	pipeline->BindRenderTargets();
}

void BloomPass::CreateRenderTargets(OnResizeNotice* resizeNotice)
{
	samplerPass->CreateDestTexture(0,"BloomSampler0");
	gaussianBlurPassX->CreateDestTexture(0,"BloomSampler0");
	gaussianBlurPassY->CreateDestTexture(0,"BloomSampler0");
	accumulatorPass->CreateDestTexture(0,"BloomSampler0");
	//accumulatorPass0->CreateDestTexture(0,"BloomSampler0");
	lightPass->CreateDestTexture(0,"BloomSampler0");
}

void BloomPass::DeleteRenderTargets()
{
	samplerPass->DeleteDestTextures();
	gaussianBlurPassX->DeleteDestTextures();
	gaussianBlurPassY->DeleteDestTextures();
	accumulatorPass->DeleteDestTextures();
	//accumulatorPass0->DeleteDestTextures();
	lightPass->DeleteDestTextures();
}

void BloomPass::CreateShader()
{
}
