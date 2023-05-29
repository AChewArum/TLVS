#pragma once

#include "Shadow.h"

#include "Resources.h"

#include "Pipeline.h"

#include "OnResize.h"

#include "Camera.h"

#include "RenderTargetTexture.h"

const float max_depth = 5000.0f;

const UINT rtSizeHigh = 10000;
const UINT rtSizeMid = 2000;
const UINT rtSizeLow = 1000;

class Shadow
{
public:
	Shadow(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice,Camera* camera, UINT directionalShadowNum);
	~Shadow();

	void Execute();
	void Execute(UINT indexCount, UINT startIndexLocation);
private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	void CalculateSizeOfFrustum(TL_Math::Vector3 LTN, TL_Math::Vector3 RTN, TL_Math::Vector3 LBN, TL_Math::Vector3 RBN,
		TL_Math::Vector3 LTF, TL_Math::Vector3 RTF, TL_Math::Vector3 LBF, TL_Math::Vector3 RBF, TL_Math::Vector3& middlePoint, float& width, float& height);
	void CalculateSizeOfFrustums();

	TL_Math::Vector3 dir = { 1,-1, 0 };//일단 빛 생각하지 말아보자

	__declspec(align(16)) struct Data
	{
		TL_Math::Matrix view;
		TL_Math::Matrix  proj;
		TL_Math::Matrix  viewInv;
		TL_Math::Matrix  projInv;
		TL_Math::Vector3 camPos;
		float frustumFar;
	};

	Resource<ID3D11SamplerState> samplerState;
	Camera* camera;
	Shader* shadowShader;//빛을 기준으로 깊이 구해주는 쉐이더
	D3D11_VIEWPORT viewPort;
	Resource<ID3D11DepthStencilState> depthState;
	Resource<ID3D11RasterizerState> rasterState;

	//High
	Resource<ID3D11Texture2D> depthStencilBufferHigh;
	Resource<ID3D11DepthStencilView> depthStencilViewHigh;
	ConstantBuffer* lightSpaceViewProjHigh;
	RenderTargetTexture* depthFromLightHigh;
	Data lightCamHigh;
	TL_Math::Matrix lightTransformHigh;

	//Mid
	Resource<ID3D11Texture2D> depthStencilBufferMid;
	Resource<ID3D11DepthStencilView> depthStencilViewMid;
	ConstantBuffer* lightSpaceViewProjMid;
	RenderTargetTexture* depthFromLightMid;
	Data lightCamMid;
	TL_Math::Matrix lightTransformMid;

	//Low
	Resource<ID3D11Texture2D> depthStencilBufferLow;
	Resource<ID3D11DepthStencilView> depthStencilViewLow;
	ConstantBuffer* lightSpaceViewProjLow;
	RenderTargetTexture* depthFromLightLow;
	Data lightCamLow;
	TL_Math::Matrix lightTransformLow;

	void CreateRTTs(OnResizeNotice* resizeNotice);
	void CreateDepthStateAndView();
	void DescViewport(float size);
	void CreateShader();
	void CreateAndSetSamplerState();
	void CreateRasterState();

	TL_Math::Vector3 axisZ;
	TL_Math::Vector3 axisX;
	TL_Math::Vector3 axisY;

public:
	void ClearRTTs();
};