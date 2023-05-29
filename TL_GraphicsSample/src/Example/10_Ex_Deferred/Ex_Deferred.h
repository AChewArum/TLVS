#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMaterial.h"
#include "Graphics/IVertex.h"
#include "Graphics\ICamera.h"
#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\TL_Box.h"

class ajwCommon::Input;
class Ex_Deferred : public IExample
{
public:

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ImGui() override;

	ajwCommon::Input* input;

	TL_Graphics::ICamera* camera;
	Transform camT;
	void CameraMove();

	TL_Box box;
	TL_Box box2;
	void BoxMove();

	//Lights
	TL_Graphics::DirectionalLight directionalLight;
	TL_Graphics::PointLight pointLight;
	TL_Graphics::SpotLight spotLight;


	__declspec(align(16)) struct Rough_Metal
	{
		float metallic = 1.0f;
		float roughness = 1.0f;
	}mat;

	TL_Graphics::IConstantBuffer* materialBuffer;
};
