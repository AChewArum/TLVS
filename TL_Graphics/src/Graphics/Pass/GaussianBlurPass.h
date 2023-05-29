#pragma once

#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

#include "ConstantBuffer.h"

/// <summary>
/// dir = 블러진행방향, (1,0) == (-1,0)
/// </summary>
class GaussianBlurPass : public IRenderPass
{
public:
	GaussianBlurPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice, TL_Math::Vector2 direction);
	~GaussianBlurPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;

	//virtual void ClearRenderTargets() override;

private:
	OnResizeNotice* resizeNotice;

	Shader* shaderPS;
	Shader* shaderPS1;

	__declspec(align(16)) struct Dir_Info
	{
		TL_Math::Vector2 dir;
	}dir_info;

	ConstantBuffer* dirInfoBuffer;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();
};