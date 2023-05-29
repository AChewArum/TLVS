#include "pch_dx_11.h"
#include "Material.h"

#include "Pipeline.h"

Material::Material(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline
	, const TL_Graphics::MaterialDesc& desc)
	: dc(dc)
	, pixelShaderName(pixelShaderName)
	, resources(resources)
	, pipeline(pipeline)
{
	//resources->pixelShaders->Get(pixelShader, pixelShaderName);

	if (desc.baseColor_opcityFilePath.length() > 0)
		baseColor_opcityMap = new Texture(dc, resources, pipeline, desc.baseColor_opcityFilePath);

	if (desc.roughness_specular_metallic_AOFilePath.length() > 0)
		roughness_specular_metallic_AOMap = new Texture(dc, resources, pipeline, desc.roughness_specular_metallic_AOFilePath);

	if (desc.normalFilePath.length() > 0)
		normalMap = new Texture(dc, resources, pipeline, desc.normalFilePath);

	if (desc.emissiveFilePath.length() > 0)
		emissiveMap = new Texture(dc, resources, pipeline, desc.emissiveFilePath);
}

Material::~Material()
{
	SAFE_DELETE(baseColor_opcityMap);
	SAFE_DELETE(roughness_specular_metallic_AOMap);
	SAFE_DELETE(normalMap);
	SAFE_DELETE(emissiveMap);
}

void Material::Set(UINT _baseColor_opcityMapSlot, UINT _roughness_specular_metallic_AOMapSlot, UINT _normalMapSlot, UINT _emssiveMapSlot)
{
	pipeline->SetMaterial(this);

	if (baseColor_opcityMap)
		baseColor_opcityMap->Set(TL_Graphics::E_SHADER_TYPE::PS, _baseColor_opcityMapSlot);

	if (roughness_specular_metallic_AOMap)
		roughness_specular_metallic_AOMap->Set(TL_Graphics::E_SHADER_TYPE::PS, _roughness_specular_metallic_AOMapSlot);

	if (normalMap)
		normalMap->Set(TL_Graphics::E_SHADER_TYPE::PS, _normalMapSlot);

	if (emissiveMap)
		emissiveMap->Set(TL_Graphics::E_SHADER_TYPE::PS, _emssiveMapSlot);
}

void Material::SetBaseColor_Opacity(TL_Graphics::IShaderResource* baseColor_opacity)
{
	baseColor_opcityMap = reinterpret_cast<Texture*>(baseColor_opacity);
}

void Material::SetRoughness_Specular_Metallic_AO(TL_Graphics::IShaderResource* roughness_specular_metallic_ao)
{
	roughness_specular_metallic_AOMap = reinterpret_cast<Texture*>(roughness_specular_metallic_ao);
}

void Material::SetNormal(TL_Graphics::IShaderResource* normal)
{
	normalMap = reinterpret_cast<Texture*>(normal);
}

void Material::SetEmissive(TL_Graphics::IShaderResource* emissive)
{
	emissiveMap = reinterpret_cast<Texture*>(emissive);
}
//
//void Material::SetAlbedo(class TL_Graphics::IShaderResource* albedo)
//{
//	albedoMap = reinterpret_cast<Texture*>(albedo);
//}
//
//void Material::SetMetallic(class TL_Graphics::IShaderResource* metallic)
//{
//	metallicMap = reinterpret_cast<Texture*>(metallic);
//}
//
//void Material::SetRoughness(class TL_Graphics::IShaderResource* roughness)
//{
//	roughnessMap = reinterpret_cast<Texture*>(roughness);
//}

//void Material::SetShader(wstring fileName)
//{
//	pixelShaderName = fileName;
//	resources->pixelShaders->Get(pixelShader, fileName);
//}
