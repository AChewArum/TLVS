#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\LayerMask.h"

namespace TL_GameEngine
{
	uint32 LayerMask::GetMask(std::vector<tstring>& layerNames) noexcept
	{
		uint32 _layerMask = 0;

		foreach(layerNameIter, tstring, layerNames)
		{
			uint32 _mask = 1 << TL_GameEngine::GameWorld::GetInstance()->FindLayerIndex(*layerNameIter);
			_layerMask |= _mask;
		}

		return _layerMask;
	}

	tstring& LayerMask::LayerToName(uint32 layer) noexcept
	{
		return GameWorld::GetInstance()->FindLayerName(layer);
	}

	uint32 TL_GameEngine::LayerMask::NameToLayer(const wchar_t* layerName) noexcept
	{
		uint32 _index = TL_GameEngine::GameWorld::GetInstance()->FindLayerIndex(layerName);

		if (_index == -1)
		{
			return 0;
		}
		else
		{
			return 1 << _index;
		}
	}

	uint32 TL_GameEngine::LayerMask::NameToLayer(tstring& layerName) noexcept
	{
		uint32 _index = TL_GameEngine::GameWorld::GetInstance()->FindLayerIndex(layerName);

		if (_index == -1)
		{
			return 0;
		}
		else
		{
			return 1 << _index;
		}
	}

	uint32 NameToLayer(const wchar_t* layerName) noexcept
	{
		uint32 _index = GameWorld::GetInstance()->FindLayerIndex(layerName);

		if (_index == -1)
		{
			return 0;
		}
		else
		{
			return 1 << _index;
		}
	}

	uint32 NameToLayer(tstring& layerName) noexcept
	{
		uint32 _index = TL_GameEngine::GameWorld::GetInstance()->FindLayerIndex(layerName);

		if (_index == -1)
		{
			return 0;
		}
		else
		{
			return 1 << _index;
		}
	}
}