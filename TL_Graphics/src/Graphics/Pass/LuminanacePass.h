#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "Pass/SamplerPass.h"

#include "Pass/AveragePass.h"

#include "Pass/GreyScalePass.h"

class LuminancePass : public IRenderPass
{
public:
	LuminancePass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~LuminancePass();

	void Init();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;

	GreyScalePass* greyScalePass;
	AveragePass* averagePass;
	AveragePass* averagePass0;

private:
	void CreateShader();
};