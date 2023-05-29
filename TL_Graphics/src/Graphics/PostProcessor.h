#pragma once

#include "ControlPanel.h"

#include "ScreenMesh.h"

#include "Pass/GBufferRenderPass.h"

#include "Pass/DeferredRenderPass.h"

#include "Pass/GridPass.h"

#include "Pass/SamplerPass.h"

#include "Pass/ColorGradingPass.h"

#include "Pass/GaussianBlurPass.h"

#include "Pass/LightAdaptionPass.h"

#include "Pass/FinalPass.h"

#include "Pass/CubeMapPass.h"

#include "Pass/BloomPass.h"

#include "Pass/LightPass.h"

#include "Pass/GreyScalePass.h"

#include "Pass/AveragePass.h"

#include "Pass/LuminanacePass.h"

#include "Pass/LightAdaptionPass.h"

#include "Pass/ToneMappingPass.h"

/// <summary>
/// 후처리 담당
/// </summary>

class PostProcessor
{
public:
	PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~PostProcessor();
	
	void CollectGBufferInfos();

	void Execute();

	TL_Graphics::ControlPanel control;

private:
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	GBufferRenderPass* gBufferRenderPass;

	GridPass* gridPass;//후처리에 해당되지 않음, screenMesh 활용하기 위해서 같이 있음

	ScreenMesh* screenMesh;

	DeferredRenderPass* deferredRenderPass;

	SamplerPass* downSamplerPass;

	ColorGradingPass* colorGradingPass;

	GaussianBlurPass* gaussianBlurPassX;
	GaussianBlurPass* gaussianBlurPassY;

	LightAdaptionPass* lightAdaptionPass;

	CubeMapPass* cubeMapPass = nullptr;

	LightPass* lightPass = nullptr;

	BloomPass* bloomPass = nullptr;

	GreyScalePass* greyScalePass = nullptr;

	AveragePass* averagePass = nullptr;

	LuminancePass* luminancePass = nullptr;

	ToneMappingPass* toneMappingPass = nullptr;

	FinalPass* finalPass;


	void UnBindGBuffers();
};