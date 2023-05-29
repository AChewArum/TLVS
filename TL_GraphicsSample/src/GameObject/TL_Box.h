#pragma once

#include "Graphics\RenderSystem.h"

#include "Helper\Transform.h"

#include "Graphics\IShader.h"

class TL_Box
{
public:
	TL_Box();
	~TL_Box();

	void Render();

	void ImGui();

private:
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;

	__declspec(align(16)) struct Material
	{
		float metallic;
		float roughness;
	}mat;

	TL_Graphics::IConstantBuffer* matInfo;

	TL_Graphics::IShader* pixelShader;

public:
	Transform transform;

	TL_Graphics::IConstantBuffer* worldBuffer;

};