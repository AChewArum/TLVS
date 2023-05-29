#pragma once

#include "ILight.h"
#include "TextureBuffer.h"

class Light
{
public:
	Light(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~Light();

	void BeginLightSet();

	void SetLight(const TL_Graphics::DirectionalLight* light);
	void SetLight(const TL_Graphics::PointLight* light);
	void SetLight(const TL_Graphics::SpotLight* light);

	void EndLightSet();

private:
	friend class Shadow;

	TextureBuffer* textureBuffer;

	const UINT maxLightNum;
	
	struct MetaData
	{
		float lightCount;//쉐이더에서 계산에 포함하는 라이트의 수, 쉐이더에서 타입 문제 때문에 float로 사용함

		TL_Math::Vector3 pad;
	}metaData;

	//directionals dirs
	//static vector<TL_Math::Vector3> dirs;
};