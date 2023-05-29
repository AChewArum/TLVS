#pragma once

#include "Common.h"
#include "GameEngine\GameFramework\Gameworld.h"

#define LAYERMASK_DEFINE

namespace TL_GameEngine
{
	struct GAME_ENGINE_API LayerMask
	{
	public:
		LayerMask() = default;
		LayerMask(const LayerMask& layerMask) = default;
		LayerMask(uint32 value) : _value(value) {}

		~LayerMask() = default;

		static uint32 GetMask(std::vector<tstring>& layerNames) noexcept;

		template <typename ... Args>
		static uint32 GetMask(Args ... args) noexcept;

		static tstring& LayerToName(uint32 layer) noexcept;

		static uint32 NameToLayer(const wchar_t* layerName) noexcept;
		static uint32 NameToLayer(tstring& layerName) noexcept;

		LayerMask operator=(const uint32& value) const noexcept;
		LayerMask operator=(const LayerMask& value) const noexcept;
		LayerMask operator|(const uint32& value) noexcept;
		LayerMask operator|(const LayerMask& value) noexcept;
		LayerMask operator&(const uint32& value) noexcept;
		LayerMask operator&(const LayerMask& value) noexcept;


		bool operator==(const uint32& value) const noexcept;
		bool operator==(const LayerMask& value) const noexcept;
		LayerMask& operator|=(const uint32& value) noexcept;
		LayerMask& operator|=(const LayerMask& value) noexcept;
		LayerMask& operator&=(const uint32& value) noexcept;
		LayerMask& operator&=(const LayerMask& value) noexcept;

		operator uint32(void) const noexcept;

		uint32 _value;
	};
}

#include "GameEngine\Gameframework\LayerMask.inl"