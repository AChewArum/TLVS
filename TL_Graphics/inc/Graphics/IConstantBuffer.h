#pragma once

#include "Export.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT IConstantBuffer
	{
	public:
		virtual void Set(TL_Graphics::E_SHADER_TYPE type,
			UINT slot) = 0;
		virtual void Update(void* data, size_t dataSize) = 0;

	};
}