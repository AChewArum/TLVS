#pragma once

#include "Math\TL_Math.h"

#include "FMOD\fmod_common.h"

namespace TL_Sound
{
	FMOD_VECTOR Vector3ToFMODVector(TL_Math::Vector3& vector);
}