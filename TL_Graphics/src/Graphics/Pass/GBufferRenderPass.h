#pragma once

#include "IRenderPass.h"

#include "OnResize.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

/// <summary>
/// Deferred에 필요한 텍스쳐를 뽑아주는 데 필요한 것들 의 묶음
/// written by Ssreppa 230110
/// </summary>

class GBufferRenderPass : public IRenderPass
{
public:
	GBufferRenderPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~GBufferRenderPass();


	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	void SetGBuffers();



private:
	Shader* shaderPS;

	Resource<ID3D11BlendState> blendState;//Deferred에 필요한 GBuffer을 뽑을 때 쓰는 BlendState
private:
	void CreateRenderTargets(OnResizeNotice* resizeNotice);

	void CreateBlendStates();

	void CreateShader();

};
