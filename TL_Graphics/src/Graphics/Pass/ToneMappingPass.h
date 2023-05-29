#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

#include "SamplerPass.h"

#include "GaussianBlurPass.h"

#include "AccumulatorPass.h"

#include "LightPass.h"

class ToneMappingPass : public IRenderPass
{
public:
	ToneMappingPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~ToneMappingPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;

private:
	void CreateRenderTargets(OnResizeNotice* resizeNotice);
	void DeleteRenderTargets();

	void CreateShader();
};