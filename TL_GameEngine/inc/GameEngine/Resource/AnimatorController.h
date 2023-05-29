#pragma once

#include "GameEngine\Object.h"

namespace TL_GameEngine
{
	class GAME_ENGINE_API AnimatorController
		: public Object
	{
		friend class Resources;

	public:
		AnimatorController() = default;

		AnimatorController(const tstring& _uuid)
	        : Object(TEXT("AnimatorController"), _uuid) {};

		AnimatorController(const tstring& _uuid, const tstring& _name)
	        : Object(TEXT("AnimatorController"), _uuid, _name) {};

		virtual ~AnimatorController() = default;
	private:

	};
}