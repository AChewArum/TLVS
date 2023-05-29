#pragma once

#include "Graphics\RenderSystem.h"

#include "Math\TL_Math.h"

#include "Helper\Transform.h"

/// <summary>
/// 간단한 클래스
/// 
/// 사용 예제 파일, StateChange.cpp
/// 
/// 사용 쉐이더, BoxVS, BoxPS
/// </summary>

class Box
{
public:
	Box();
	~Box();

	void Render();

	void UpdateColor(TL_Math::Vector4 color);

	void UpdatePos(TL_Math::Vector3 pos);

private:
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;

	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer;

	TL_Math::Vector4 color;
	TL_Graphics::IConstantBuffer* colorBuffer;
};