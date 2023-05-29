#pragma once

#include "Export.h"

#include "ShaderType.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT IRenderTargetTexture
	{
	public:
		virtual ~IRenderTargetTexture() {};

		virtual void SetRT(UINT slot) = 0;
		virtual void Clear(TL_Math::Vector4 color = {0,0,0,0}) = 0;

		virtual void SetT(TL_Graphics::E_SHADER_TYPE type, UINT slot) = 0;
	};
}