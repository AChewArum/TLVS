#pragma once

#include <string>
#include "Export.h"

#include "Math\TL_Math.h"


namespace TL_Graphics
{
	struct MaterialDesc
	{
		std::wstring baseColor_opcityFilePath = L"";
		std::wstring roughness_specular_metallic_AOFilePath = L"";
		std::wstring normalFilePath = L"";
		std::wstring emissiveFilePath = L"";
	};

	class AJW_GRAPHICS_DLLEXPORT IMaterial
	{
	public:
		virtual void Set(UINT baseColor_opcityMapSlot = 0, UINT roughness_specular_metallic_AOMapSlot = 1, UINT normalMapSlot = 2, UINT emssiveMapSlot = 3) = 0;
		
		virtual void SetBaseColor_Opacity(class IShaderResource*) = 0;
		virtual void SetRoughness_Specular_Metallic_AO(class IShaderResource*) = 0;
		virtual void SetNormal(class IShaderResource* ) = 0;
		virtual void SetEmissive(class IShaderResource* ) = 0;

	};
}