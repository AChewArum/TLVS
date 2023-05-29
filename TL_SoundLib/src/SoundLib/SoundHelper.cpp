#include "SoundLib\SoundPCH.h"

namespace TL_Sound
{
	FMOD_VECTOR Vector3ToFMODVector(TL_Math::Vector3& vector)
	{
		FMOD_VECTOR _ret;

		_ret.x = vector.x;
		_ret.y = vector.y;
		_ret.z = vector.z;

		return _ret;
	}

}