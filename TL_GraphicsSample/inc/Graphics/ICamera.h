#pragma once

#include "Export.h"

#include "Math\TL_Math.h"


namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT ICamera
	{
	public:
		virtual void Set() = 0;
		virtual void Update(TL_Math::Vector3 pos, TL_Math::Vector3 rot) = 0;
		virtual void Update(TL_Math::Matrix m) = 0;
	};
}