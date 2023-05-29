#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class SamplerPass : public IRenderPass
{
public:
	SamplerPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, Shader* shaderPS = nullptr);


	~SamplerPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	Texture* GetDestTexture() { return rtts[0]; }

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);
	void CreateShader();

	bool isShaderCreatedHere = false;
};