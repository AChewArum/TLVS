#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

class ajwCommon::Input;
class Ex_Texture : public IExample
{
public:

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	ajwCommon::Input* input;

	TL_Graphics::ICamera* camera;
	Transform camT;
	void CameraMove();


#pragma region square
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;
	TL_Graphics::VertexAttribute vertexAttribute;

	Transform squareT;
	TL_Graphics::IConstantBuffer* worldBuffer;
	void TransformMove();

	TL_Graphics::ITexture* texture;
#pragma endregion


};
