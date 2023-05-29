#include "pch_dx_11.h"
#include "Light.h"

#include "ShaderResourceSlotPS.h"

Light::Light(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
	:maxLightNum (100)
{
	textureBuffer = new TextureBuffer(dc, resources, pipeline, nullptr, sizeof(MetaData) + sizeof(TL_Graphics::DirectionalLight) * maxLightNum,"Light");
}

Light::~Light()
{
	SAFE_DELETE(textureBuffer);
}

void Light::BeginLightSet()
{
	textureBuffer->StartPartialUpdate();
	metaData.lightCount = 0;
}

void Light::SetLight(const TL_Graphics::DirectionalLight* light)
{
	if (metaData.lightCount >= maxLightNum) return;

	textureBuffer->PartialUpdate(sizeof(MetaData) + sizeof(TL_Graphics::DirectionalLight) * metaData.lightCount++, (void*)light, sizeof(TL_Graphics::DirectionalLight));

	//dirs.push_back(light->direction);
}

void Light::SetLight(const TL_Graphics::PointLight* light)
{
	if (metaData.lightCount >= maxLightNum) return;

	textureBuffer->PartialUpdate(sizeof(MetaData) + sizeof(TL_Graphics::PointLight) * metaData.lightCount++, (void*)light, sizeof(TL_Graphics::PointLight));
}

void Light::SetLight(const TL_Graphics::SpotLight* light)
{
	if (metaData.lightCount >= maxLightNum) return;

	textureBuffer->PartialUpdate(sizeof(MetaData) + sizeof(TL_Graphics::SpotLight) * metaData.lightCount++, (void*)light, sizeof(TL_Graphics::SpotLight));
}

void Light::EndLightSet()
{
	textureBuffer->PartialUpdate(0, &metaData, sizeof(MetaData));//맨앞에 라이트 갯수 적는다.
	textureBuffer->EndPartialUpdate();

	textureBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, LightsSlot);
}
