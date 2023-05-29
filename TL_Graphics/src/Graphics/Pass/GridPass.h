#pragma once

#include "IRenderPass.h"

#include "OnResize.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

/// <summary>
/// Deferred에 필요한 텍스쳐를 뽑아주는 데 필요한 것들 의 묶음
/// </summary>

class GridPass : public IRenderPass
{
public:
	GridPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~GridPass();


	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;
	virtual void Execute() override;


private:

	Shader* shaderPS;
private:
	void CreateRenderTarget(OnResizeNotice* resizeNotice);

	void CreateShader();

};
