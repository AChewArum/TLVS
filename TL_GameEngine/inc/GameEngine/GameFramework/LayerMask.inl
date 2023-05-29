#pragma once
//#ifndef LAYERMASK_DEFINE
//#include "GameEngine\GameFramework\LayerMask.h"
//#endif
//#ifndef LAYERMASK_INLINE
//#define LAYERMASK_INLINE

namespace TL_GameEngine
{
	template<typename ...Args>
	inline uint32 LayerMask::GetMask(Args ...args) noexcept
	{
		std::vector<std::string> layerNames{ args ... };

		uint32 _layerMask;

		foreach(layerNameIter, tstring, layerNames)
		{
			uint32 _mask = 1 << TL_GameEngine::GameWorld::GetInstance()->FindLayerIndex(*layerNameIter);
			_layerMask |= _mask;
		}

		return _layerMask;
	}

	inline LayerMask LayerMask::operator=(const uint32& value) const noexcept
	{
		return LayerMask(value);
	}

	inline LayerMask LayerMask::operator=(const LayerMask& value) const noexcept
	{
		return LayerMask(value);
	}

	inline LayerMask LayerMask::operator|(const uint32& value) noexcept
	{
		_value |= value;

		return LayerMask(_value);
	}

	inline LayerMask LayerMask::operator|(const LayerMask& value) noexcept
	{
		_value |= value._value;

		return LayerMask(_value);
	}

	inline LayerMask LayerMask::operator&(const uint32& value) noexcept
	{
		_value &= value;

		return LayerMask(_value);
	}

	inline LayerMask LayerMask::operator&(const LayerMask& value) noexcept
	{
		_value &= value._value;

		return LayerMask(_value);
	}

	inline bool TL_GameEngine::LayerMask::operator==(const uint32& value) const noexcept
	{
		return _value == value;
	}

	inline bool TL_GameEngine::LayerMask::operator==(const LayerMask& value) const noexcept
	{
		return _value == value._value;
	}

	inline LayerMask& TL_GameEngine::LayerMask::operator|=(const uint32& value) noexcept
	{
		_value |= value;

		return *this;
	}

	inline LayerMask& TL_GameEngine::LayerMask::operator|=(const LayerMask& value) noexcept
	{
		_value |= value._value;

		return *this;
	}

	inline LayerMask& TL_GameEngine::LayerMask::operator&=(const uint32& value) noexcept
	{
		_value &= value;

		return *this;
	}

	inline LayerMask& TL_GameEngine::LayerMask::operator&=(const LayerMask& value) noexcept
	{
		_value &= value._value;

		return *this;
	}

	inline LayerMask::operator uint32(void) const noexcept
	{
		return static_cast<uint32>(_value);
	}
}
//#endif