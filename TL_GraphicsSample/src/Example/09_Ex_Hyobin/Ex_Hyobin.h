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

#include "FBXLibrary\FBXParserData.h"
#include "FBXLibrary/FBXModelLoader.h"

#include "imgui.h"

/// <summary>
/// Hyobin 누나의 FBX Loader Test 해보는 Scene
/// 
/// this example needs library 'TL_FBXLibrary' written by 임효빈
/// </summary>
class ajwCommon::Input;

class Ex_Hyobin : public IExample
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

	void BoxMove();


	TL_FBXLibrary::FBXModelLoader* m_FBXLoader;

	/// obj
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;
	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer;


	TL_Graphics::DirectionalLight directionalLight;

	__declspec(align(16)) struct Rough_Metal
	{
		float metallic = 1.0f;
		float roughness = 1.0f;
	}mat;

	TL_Graphics::IConstantBuffer* materialBuffer;

	TL_Graphics::ControlPanel* control;

	void TestSkinning();
	void TestStatic();
	void TestTL();


	__declspec(align(16)) struct Test
	{
		float flt = 1.0f;
	}test;

	TL_Graphics::IConstantBuffer* testBuffer;


	//TL_Graphics::ITexture* cubeMap;
	//TL_Graphics::ITexture* irradianceMap;
	//TL_Graphics::ITexture* prefilteredEnvMap;
	//TL_Graphics::ITexture* iblBRDF;


};
