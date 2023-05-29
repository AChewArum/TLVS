#pragma once

#include "Export.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT ITexture
	{
	public:
		virtual ~ITexture() {};

		virtual void Set(TL_Graphics::E_SHADER_TYPE type,
			UINT slot) = 0;

	};
}