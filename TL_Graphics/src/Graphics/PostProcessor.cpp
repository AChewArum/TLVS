#include "pch_dx_11.h"
#include "PostProcessor.h"

#include "Pipeline.h"

PostProcessor::PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{

	gBufferRenderPass = new GBufferRenderPass(dc, resources, pipeline, resizeNotice);

	screenMesh = new ScreenMesh(dc, resources, pipeline);

	deferredRenderPass = new DeferredRenderPass(dc, resources, pipeline, resizeNotice);

	gridPass = new GridPass(dc, resources, pipeline, resizeNotice);

	downSamplerPass = new SamplerPass(dc, resources, pipeline, resizeNotice);
	//downSamplerPass->SetRatio(0.5f, 0.5f);

	colorGradingPass = new ColorGradingPass(dc, resources, pipeline, resizeNotice);

	gaussianBlurPassX = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 1,0 });

	gaussianBlurPassY = new GaussianBlurPass(dc, resources, pipeline, resizeNotice, { 0,1 });

	lightAdaptionPass = new LightAdaptionPass(dc, resources, pipeline, resizeNotice);

	finalPass = new FinalPass(dc, resources, pipeline, resizeNotice);

	cubeMapPass = new CubeMapPass(dc, resources, pipeline, resizeNotice, L"_DevelopmentAssets/Texture/CubeMaps/ValleyEnvHDR.dds");

	bloomPass = new BloomPass(dc, resources, pipeline, resizeNotice);
	bloomPass->CreateDestTexture();

	lightPass = new LightPass(dc, resources, pipeline, resizeNotice);

	greyScalePass = new GreyScalePass(dc, resources, pipeline, resizeNotice);
	greyScalePass->CreateDestTexture(0, "GreyScale", DXGI_FORMAT_R32_FLOAT);

	averagePass = new AveragePass(dc, resources, pipeline, resizeNotice);
	averagePass->CreateDestTexture(0, "Average");

	luminancePass = new LuminancePass(dc, resources, pipeline, resizeNotice);
	luminancePass->CreateDestTexture(0, "Luminance");
	luminancePass->Init();

	lightAdaptionPass = new LightAdaptionPass(dc, resources, pipeline, resizeNotice);
	lightAdaptionPass->CreateDestTexture(0, "LightAdaption");

	toneMappingPass = new ToneMappingPass(dc, resources, pipeline, resizeNotice);
	toneMappingPass->CreateDestTexture(0, "toneMapping");
}

PostProcessor::~PostProcessor()
{
	bloomPass->DeleteDestTextures();

	SAFE_DELETE(toneMappingPass);

	SAFE_DELETE(lightAdaptionPass);

	SAFE_DELETE(averagePass);

	SAFE_DELETE(luminancePass);

	SAFE_DELETE(greyScalePass);

	SAFE_DELETE(gBufferRenderPass);

	SAFE_DELETE(finalPass);

	SAFE_DELETE(lightPass);

	SAFE_DELETE(bloomPass);

	SAFE_DELETE(lightAdaptionPass);

	SAFE_DELETE(cubeMapPass);

	SAFE_DELETE(gaussianBlurPassX);
	SAFE_DELETE(gaussianBlurPassY);
	SAFE_DELETE(colorGradingPass);
	SAFE_DELETE(downSamplerPass);
	SAFE_DELETE(gridPass);
	SAFE_DELETE(deferredRenderPass);
	SAFE_DELETE(screenMesh);
}

void PostProcessor::CollectGBufferInfos()
{
	gBufferRenderPass->ClearRenderTargets();
	gBufferRenderPass->Set();//화면 기하정보 뽑아냄
}

void PostProcessor::Execute()
{
	pipeline->SetDepthDisabled();
	auto oldDSView = pipeline->SetDepthStencilView(nullptr);

	cubeMapPass->Execute();
	RenderTargetTexture* cubeMap = cubeMapPass->GetDestTexture();

	screenMesh->Set();

	for (UINT i = 0; i < 8; i++)
		deferredRenderPass->SetSourceTexture(gBufferRenderPass->GetDestTexture(i), i);
	deferredRenderPass->SetDestTexture(cubeMap);
	deferredRenderPass->Execute();
	RenderTargetTexture* proto = deferredRenderPass->GetDestTexture();



	///여기부터 후처리
	RenderTargetTexture* before = proto;

	if (control.doColorGrading)
		colorGradingPass->Execute();


	//lightPass->SetSourceTexture(before);
	//lightPass->Execute();
	//before = lightPass->GetDestTexture();

	//bloomPass->SetDestTexture(before);

	/*greyScalePass->SetSourceTexture(before);
	greyScalePass->Execute();
	before = greyScalePass->GetDestTexture();*/

	/*averagePass->SetRatio(0.09f, 0.09f);
	averagePass->SetSourceTexture(before);
	averagePass->Execute();
	before = averagePass->GetDestTexture();*/

	{
		luminancePass->SetSourceTexture(before);
		luminancePass->Execute();
		Texture* luminance = luminancePass->GetDestTexture();

		lightAdaptionPass->SetSourceTexture(before, 0);
		lightAdaptionPass->SetSourceTexture(luminance, 1);
		lightAdaptionPass->Execute();
		before = lightAdaptionPass->GetDestTexture();
	}

	toneMappingPass->SetSourceTexture(before, 0);
	toneMappingPass->Execute();
	before = toneMappingPass->GetDestTexture();

	bloomPass->SetSourceTexture(before, 0);
	bloomPass->Execute();
	before = bloomPass->GetDestTexture();




	finalPass->SetSourceTexture(before);
	finalPass->Execute();







	gBufferRenderPass->ClearRenderTargets();
	//정리
	UnBindGBuffers();
	pipeline->SetDepthStencilView(oldDSView);
}

void PostProcessor::UnBindGBuffers()
{
	for (int i = 0; i < 8; i++)
	{
		pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 20 + i);
	}

	pipeline->SetShaderResource(nullptr,
		TL_Graphics::E_SHADER_TYPE::PS, 31);

	pipeline->BindShaderResourcesPS();
}



//void PostProcessor::Execute()
//{
//	Texture* before = nullptr;
//
//	pipeline->SetDepthDisabled();
//
//	auto oldDSView = pipeline->SetDepthStencilView(nullptr);
//
//	cubeMapPass->Execute();
//	Texture* cubeMap = cubeMapPass->GetDestTexture();
//
//	screenMesh->Set();
//
//
//	if (control.doGrid)
//		gridPass->Execute();
//
//	deferredRenderPass->Execute();
//	before = deferredRenderPass->GetDestTexture();
//
//	if (control.doDownSample)
//	{
//		downSamplerPass->SetSourceTexture(before);
//		downSamplerPass->Execute();
//	}
//
//	if (control.doColorGrading)
//		colorGradingPass->Execute();
//
//	if (control.doGaussianBlur)
//	{
//		gaussianBlurPassX->Execute();
//		gaussianBlurPassY->Execute();
//	}
//	/*if (control.doDownSample)
//
//	lightAdaptionPass->Execute();*/
//
//	finalPass->SetSourceTexture(cubeMap);
//	finalPass->Execute();
//
//	//정리
//	UnBindGBuffers();
//	pipeline->SetDepthStencilView(oldDSView);
//}
