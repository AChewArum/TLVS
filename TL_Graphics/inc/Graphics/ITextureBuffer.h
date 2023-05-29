#pragma once

#include "Export.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT ITextureBuffer
	{
	public:
		virtual ~ITextureBuffer() {};

		virtual void Set(TL_Graphics::E_SHADER_TYPE type,
			UINT slot) = 0;

		virtual void Update(void* data, size_t dataSize) = 0;

		//map, unmap
		virtual void StartPartialUpdate() = 0;
		virtual void PartialUpdate(size_t offset, void* data, size_t dataSize) = 0;
		virtual void EndPartialUpdate() = 0;

	};
}