#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\Box.h"

class ajwCommon::Input;
class EX_RTT : public IExample
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

	TL_Graphics::IRenderTargetTexture* rtt[4];

	Box a, b, c;

	//canvas
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;


	TL_Graphics::ITexture* texture;

};
