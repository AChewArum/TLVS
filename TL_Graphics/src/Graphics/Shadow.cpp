#include "pch_dx_11.h"
#include "Shadow.h"

Shadow::Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, Camera* camera, UINT directionalShadowNum)
	: dc(dc)
	, resources(resources)
	, pipeline(pipeline)
	, camera(camera)
{
	lightSpaceViewProjHigh = new ConstantBuffer(dc, resources, pipeline, &lightCamHigh, sizeof(Data));
	lightSpaceViewProjHigh->SetDebugName("LightCamHigh");
	lightSpaceViewProjMid = new ConstantBuffer(dc, resources, pipeline, &lightCamMid, sizeof(Data));
	lightSpaceViewProjMid->SetDebugName("LightCamMid");
	lightSpaceViewProjLow = new ConstantBuffer(dc, resources, pipeline, &lightCamLow, sizeof(Data));
	lightSpaceViewProjLow->SetDebugName("LightCamLow");

	CreateRTTs(resizeNotice);
	CreateDepthStateAndView();
	CreateShader();
	CreateAndSetSamplerState();
	CreateRasterState();

	dir.Normalize();

	//ÁÂÇ¥°è ¸Â°í
	axisZ = dir;
	axisX =  TL_Math::Vector3(0, 1, 0).Cross(axisZ);
	axisY = axisZ.Cross(axisX);

	axisX.Normalize();
	axisY.Normalize();
}

Shadow::~Shadow()
{
	SAFE_DELETE(lightSpaceViewProjHigh);
	SAFE_DELETE(lightSpaceViewProjMid);
	//SAFE_DELETE(lightSpaceViewProjLow);
	SAFE_DELETE(depthFromLightHigh);
	SAFE_DELETE(depthFromLightMid);
	//SAFE_DELETE(depthFromLightLow);
	SAFE_DELETE(shadowShader);
}

void Shadow::CalculateSizeOfFrustum(TL_Math::Vector3 LTN, TL_Math::Vector3 RTN, TL_Math::Vector3 LBN, TL_Math::Vector3 RBN,	TL_Math::Vector3 LTF, TL_Math::Vector3 RTF, TL_Math::Vector3 LBF, TL_Math::Vector3 RBF, TL_Math::Vector3& middlePoint, float& width, float& height)
{
	middlePoint = {};

	width = 0;
	height = 0;
	//ºûÀÇ ¿ª¹æÇâÀ» ÃÄ´Ù º¸°í ÀÖ´Â ÁÂÇ¥°è¸¦ ±¸ÇÔ

	lightTransformHigh = TL_Math::Matrix(axisX, axisY, axisZ);


	{//ºû ÁÂÇ¥°è xÃàÀ¸·Î Åõ¿µ
		float max = -FLT_MAX;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

		axisScalar = axisX.Dot(LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisX.Dot(RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		width = max - min;

		width *= 1.1f;

		TL_Math::Vector3 temp = XMVectorScale(axisX, (max + min) * 0.5f);

		middlePoint += temp;
	}

	{//ºû ÁÂÇ¥°è yÃàÀ¸·Î Åõ¿µ
		float max = -FLT_MAX;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

		axisScalar = axisY.Dot(LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisY.Dot(RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		height = max - min;

		height *= 1.1f;

		TL_Math::Vector3 temp = XMVectorScale(axisY, (max + min) * 0.5f);

		middlePoint += temp;
	}

	{//ºû ÁÂÇ¥°è zÃàÀ¸·Î Åõ¿µ
		float max = -FLT_MAX;
		float min = FLT_MAX;

		float axisScalar = 0.0f;

		axisScalar = axisZ.Dot(LTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(LBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(RTF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(RBF);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(LTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(LBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(RTN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		axisScalar = axisZ.Dot(RBN);
		max = max > axisScalar ? max : axisScalar;
		min = min < axisScalar ? min : axisScalar;

		//depth = max - min;

		//frustumÀÇ ¿Ü°ûÀ¸·Î °¡°Ô²û
		TL_Math::Vector3 temp = XMVectorScale(axisZ, max - max_depth);

		middlePoint += temp;
	}
}

void Shadow::CalculateSizeOfFrustums()
{
	float div1 = 0.3f;
	float div2 = 0.6f;
	float div3 = 1.0f;

	TL_Math::Vector3 GapLT = camera->worldPoints.LTF - camera->data.camPos;
	TL_Math::Vector3 GapRT = camera->worldPoints.RTF - camera->data.camPos;
	TL_Math::Vector3 GapLB = camera->worldPoints.LBF - camera->data.camPos;
	TL_Math::Vector3 GapRB = camera->worldPoints.RBF - camera->data.camPos;

	TL_Math::Vector3 LT0 = camera->data.camPos;
	TL_Math::Vector3 RT0 = camera->data.camPos;
	TL_Math::Vector3 LB0 = camera->data.camPos;
	TL_Math::Vector3 RB0 = camera->data.camPos;

	TL_Math::Vector3 LT1 = LT0 + div1 * GapLT;
	TL_Math::Vector3 RT1 = RT0 + div1 * GapRT;
	TL_Math::Vector3 LB1 = LB0 + div1 * GapLB;
	TL_Math::Vector3 RB1 = RB0 + div1 * GapRB;

	TL_Math::Vector3 LT2 = LT0 + div2 * GapLT;
	TL_Math::Vector3 RT2 = RT0 + div2 * GapRT;
	TL_Math::Vector3 LB2 = LB0 + div2 * GapLB;
	TL_Math::Vector3 RB2 = RB0 + div2 * GapRB;

	TL_Math::Vector3 LT3 = LT0 + div3 * GapLT;
	TL_Math::Vector3 RT3 = RT0 + div3 * GapRT;
	TL_Math::Vector3 LB3 = LB0 + div3 * GapLB;
	TL_Math::Vector3 RB3 = RB0 + div3 * GapRB;
	//¿©±â±îÁö¸Â°í
	TL_Math::Vector3 middlePoint;
	float width = 0, height = 0;

	//High
	CalculateSizeOfFrustum(LT0, RT0, LB0, RB0, LT2, RT2, LB2, RB2, middlePoint, width, height);

	lightTransformHigh = TL_Math::Matrix(axisX, axisY, axisZ);

	lightTransformHigh.m[3][0] = middlePoint.x;
	lightTransformHigh.m[3][1] = middlePoint.y;
	lightTransformHigh.m[3][2] = middlePoint.z;

	lightCamHigh.viewInv = lightTransformHigh;
	lightCamHigh.view = XMMatrixInverse(nullptr, lightCamHigh.viewInv);
	lightCamHigh.proj = XMMatrixOrthographicLH(width, height, 1.0f, max_depth);
	lightCamHigh.projInv = XMMatrixInverse(nullptr, lightCamHigh.proj);
	lightCamHigh.camPos = middlePoint;
	lightCamHigh.frustumFar = max_depth;

	////Mid
	CalculateSizeOfFrustum(LT1, RT1, LB1, RB1, LT2, RT2, LB2, RB2, middlePoint, width, height);

	lightTransformMid = TL_Math::Matrix(axisX, axisY, axisZ);

	lightTransformMid.m[3][0] = middlePoint.x;
	lightTransformMid.m[3][1] = middlePoint.y;
	lightTransformMid.m[3][2] = middlePoint.z;

	lightCamMid.viewInv = lightTransformMid;
	lightCamMid.view = XMMatrixInverse(nullptr, lightCamMid.viewInv);
	lightCamMid.proj = XMMatrixOrthographicLH(width, height, 1.0f, max_depth);
	lightCamMid.projInv = XMMatrixInverse(nullptr, lightCamMid.proj);
	lightCamMid.camPos = middlePoint;
	lightCamMid.frustumFar = max_depth;


	//Low
	CalculateSizeOfFrustum(LT2, RT2, LB2, RB2, LT3, RT3, LB3, RB3, middlePoint, width, height);

	lightTransformLow = TL_Math::Matrix(axisX, axisY, axisZ);

	lightTransformLow.m[3][0] = middlePoint.x;
	lightTransformLow.m[3][1] = middlePoint.y;
	lightTransformLow.m[3][2] = middlePoint.z;

	lightCamLow.viewInv = lightTransformLow;
	lightCamLow.view = XMMatrixInverse(nullptr, lightCamLow.viewInv);
	lightCamLow.proj = XMMatrixOrthographicLH(width, height, 1.0f, max_depth);
	lightCamLow.projInv = XMMatrixInverse(nullptr, lightCamLow.proj);
	lightCamLow.camPos = middlePoint;
	lightCamLow.frustumFar = max_depth;
}

void Shadow::ClearRTTs()
{
	depthFromLightHigh->Clear({ 1,1,1,1 });
	depthFromLightMid->Clear({ 1,1,1,1 });
	depthFromLightLow->Clear({ 1,1,1,1 });

	dc->ClearDepthStencilView(depthStencilViewHigh, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->ClearDepthStencilView(depthStencilViewMid, D3D11_CLEAR_DEPTH, 1.0f, 0);
	dc->ClearDepthStencilView(depthStencilViewLow, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Shadow::CreateRTTs(OnResizeNotice* resizeNotice)
{
	depthFromLightHigh = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, rtSizeHigh, rtSizeHigh, "depthFromLightHigh", DXGI_FORMAT_R32_FLOAT);
	depthFromLightMid = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, rtSizeMid, rtSizeMid, "depthFromLightMid", DXGI_FORMAT_R32_FLOAT);
	depthFromLightLow = new RenderTargetTexture(dc, resources, pipeline, resizeNotice, rtSizeLow, rtSizeLow, "depthFromLightLow", DXGI_FORMAT_R32_FLOAT);
}

void Shadow::Execute()
{
	CalculateSizeOfFrustums();

	ID3D11RasterizerState* oldRasteriszerState = pipeline->SetCurrentRasterState(rasterState);
	ID3D11PixelShader* oldPiexelShader = nullptr;
	oldPiexelShader = (ID3D11PixelShader*)shadowShader->SetTest();

	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 16);
	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 17);
	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 18);
	pipeline->BindShaderResourcesPS();

	//High
	DescViewport(rtSizeHigh);
	D3D11_VIEWPORT* oldViewPort = pipeline->SetViewPort(&viewPort);

	ID3D11RenderTargetView* oldTargets[8] = {};
	oldTargets[0] = depthFromLightHigh->SetRTTEST(0);
	for (int i = 1; i < 8; i++)
	{
		oldTargets[i] = pipeline->SetRenderTarget(nullptr, i);
	}
	ID3D11DepthStencilView* oldDSView = pipeline->SetDepthStencilView(depthStencilViewHigh);
	//pipeline->BindRenderTargets();


	lightSpaceViewProjHigh->Update(&lightCamHigh, sizeof(Data));
	ID3D11Buffer* oldConstBuffer = lightSpaceViewProjHigh->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjHigh->Set(TL_Graphics::E_SHADER_TYPE::PS, 4);

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	depthFromLightHigh->SetT(TL_Graphics::E_SHADER_TYPE::PS, 16);

	//Mid
	DescViewport(rtSizeMid);
	pipeline->SetViewPort(&viewPort);

	depthFromLightMid->SetRTTEST(0);
	
	pipeline->SetDepthStencilView(depthStencilViewMid);

	lightSpaceViewProjMid->Update(&lightCamMid, sizeof(Data));
	lightSpaceViewProjMid->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjMid->Set(TL_Graphics::E_SHADER_TYPE::PS, 5);

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	depthFromLightMid->SetT(TL_Graphics::E_SHADER_TYPE::PS, 17);

	////Low
	DescViewport(rtSizeLow);
	pipeline->SetViewPort(&viewPort);

	depthFromLightLow->SetRTTEST(0);
	
	pipeline->SetDepthStencilView(depthStencilViewLow);

	lightSpaceViewProjLow->Update(&lightCamLow, sizeof(Data));
	lightSpaceViewProjLow->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjLow->Set(TL_Graphics::E_SHADER_TYPE::PS, 6);

	pipeline->Draw();

	pipeline->UnSetRenderTarget(0);
	depthFromLightLow->SetT(TL_Graphics::E_SHADER_TYPE::PS, 18);


	//Return To oldStates
	pipeline->SetViewPort(oldViewPort);
	pipeline->SetConstantBuffer(oldConstBuffer, TL_Graphics::E_SHADER_TYPE::VS, 0);
	pipeline->SetCurrentRasterState(oldRasteriszerState);
	for (int i = 0; i < 8; i++)
	{
		pipeline->SetRenderTarget(oldTargets[i], i);
	}
	pipeline->SetDepthStencilView(oldDSView);
	pipeline->SetShader(oldPiexelShader);
}

void Shadow::Execute(UINT indexCount, UINT startIndexLocation)
{
	CalculateSizeOfFrustums();

	ID3D11RasterizerState* oldRasteriszerState = pipeline->SetCurrentRasterState(rasterState);
	ID3D11PixelShader* oldPiexelShader = nullptr;
	oldPiexelShader = (ID3D11PixelShader*)shadowShader->SetTest();

	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 16);
	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 17);
	pipeline->SetShaderResource(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 18);
	pipeline->BindShaderResourcesPS();

	//High
	DescViewport(rtSizeHigh);
	D3D11_VIEWPORT* oldViewPort = pipeline->SetViewPort(&viewPort);

	ID3D11RenderTargetView* oldTargets[8] = {};
	oldTargets[0] = depthFromLightHigh->SetRTTEST(0);
	for (int i = 1; i < 8; i++)
	{
		oldTargets[i] = pipeline->SetRenderTarget(nullptr, i);
	}
	ID3D11DepthStencilView* oldDSView = pipeline->SetDepthStencilView(depthStencilViewHigh);
	//pipeline->BindRenderTargets();


	lightSpaceViewProjHigh->Update(&lightCamHigh, sizeof(Data));
	ID3D11Buffer* oldConstBuffer = lightSpaceViewProjHigh->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjHigh->Set(TL_Graphics::E_SHADER_TYPE::PS, 4);

	pipeline->Draw(indexCount, startIndexLocation);

	pipeline->UnSetRenderTarget(0);
	depthFromLightHigh->SetT(TL_Graphics::E_SHADER_TYPE::PS, 16);

	//Mid
	DescViewport(rtSizeMid);
	pipeline->SetViewPort(&viewPort);

	depthFromLightMid->SetRTTEST(0);

	pipeline->SetDepthStencilView(depthStencilViewMid);

	lightSpaceViewProjMid->Update(&lightCamMid, sizeof(Data));
	lightSpaceViewProjMid->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjMid->Set(TL_Graphics::E_SHADER_TYPE::PS, 5);

	pipeline->Draw(indexCount, startIndexLocation);

	pipeline->UnSetRenderTarget(0);
	depthFromLightMid->SetT(TL_Graphics::E_SHADER_TYPE::PS, 17);

	////Low
	DescViewport(rtSizeLow);
	pipeline->SetViewPort(&viewPort);

	depthFromLightLow->SetRTTEST(0);

	pipeline->SetDepthStencilView(depthStencilViewLow);

	lightSpaceViewProjLow->Update(&lightCamLow, sizeof(Data));
	lightSpaceViewProjLow->SetTest(TL_Graphics::E_SHADER_TYPE::VS, 0);
	lightSpaceViewProjLow->Set(TL_Graphics::E_SHADER_TYPE::PS, 6);

	pipeline->Draw(indexCount, startIndexLocation);

	pipeline->UnSetRenderTarget(0);
	depthFromLightLow->SetT(TL_Graphics::E_SHADER_TYPE::PS, 18);


	//Return To oldStates
	pipeline->SetViewPort(oldViewPort);
	pipeline->SetConstantBuffer(oldConstBuffer, TL_Graphics::E_SHADER_TYPE::VS, 0);
	pipeline->SetCurrentRasterState(oldRasteriszerState);
	for (int i = 0; i < 8; i++)
	{
		pipeline->SetRenderTarget(oldTargets[i], i);
	}
	pipeline->SetDepthStencilView(oldDSView);
	pipeline->SetShader(oldPiexelShader);
}

void Shadow::CreateDepthStateAndView()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = rtSizeHigh;
	depthBufferDesc.Height = rtSizeHigh;
	//depthBufferDesc.Width = width;
	//depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	resources->texture2Ds->Create(depthStencilBufferHigh, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilViewHigh, depthStencilBufferHigh);


	depthBufferDesc.Width = rtSizeMid;
	depthBufferDesc.Height = rtSizeMid;

	resources->texture2Ds->Create(depthStencilBufferMid, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilViewMid, depthStencilBufferMid);

	depthBufferDesc.Width = rtSizeLow;
	depthBufferDesc.Height = rtSizeLow;

	resources->texture2Ds->Create(depthStencilBufferLow, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilViewLow, depthStencilBufferLow);
}

void Shadow::DescViewport(float size)
{
	viewPort.Width = (float)size;
	viewPort.Height = (float)size;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
}

void Shadow::CreateShader()
{
	shadowShader = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_DepthFromLight.hlsl", "LightDepth");
}

void Shadow::CreateAndSetSamplerState()
{
	D3D11_SAMPLER_DESC desc = {};

	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;

	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

	resources->samplerStates->Get(samplerState, desc);

	pipeline->SetCurrentSamplerState(samplerState, 4);
}

void Shadow::CreateRasterState()
{
	D3D11_RASTERIZER_DESC desc = {};

	desc.DepthBias = 20000;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 1.0f;
	desc.DepthClipEnable = true;

	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;

	resources->rasterStates->Get(rasterState, desc);
}
