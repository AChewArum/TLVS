#pragma once

#include "Export.h"

#include "Math\TL_Math.h"

#include "ShaderType.h"

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT ICamera
	{
	public:
		virtual void Set(TL_Graphics::E_SHADER_TYPE type = TL_Graphics::E_SHADER_TYPE::VS,
		UINT slot = 0) = 0;
		virtual void Update(TL_Math::Vector3 pos, TL_Math::Vector3 rot) = 0;
		virtual void Update(TL_Math::Vector3 pos, TL_Math::Quaternion rot) = 0;
		virtual void Update(TL_Math::Matrix m) = 0;

	};
}