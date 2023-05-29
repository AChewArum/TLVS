#pragma once

///그냥 스왑체인에 그려주는(Sampling) 패스,
///패스들을 묶어서 마지막꺼로 저절로 되게 할 수도 있겠지만
/// 약간? 복잡하거나 애매한 부분이 있을 수도 있을 것 같아서 일단
/// 이렇게 함



#include "IRenderPass.h"

#include "RenderTargetTexture.h"

#include "Shader.h"

class FinalPass : public IRenderPass
{
public:
	FinalPass(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice);
	~FinalPass();

	// IRenderPass을(를) 통해 상속됨
	virtual void Set() override;

	virtual void Execute() override;


private:

	Shader* shaderPS;
private:
	void CreateShader();
};