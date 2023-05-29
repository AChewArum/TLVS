#pragma once

#include "Common_min.h"
#include "Graphics\ILine.h"
#include "GameEngine/internal/dll.h"

namespace TL_GameEngine
{
	struct Line
	{
		TL_Graphics::ILine* m_Line = {};
		double m_Time = {};
		float m_Duration = {};
	};

	class GAME_ENGINE_API Debug
	{
		DECLARE_SINGLETON_CLASS(Debug)

	public:
		static void DrawCube(
			TL_Math::Vector3 _worldPosition,
			TL_Math::Quaternion _worldRotation = TL_Math::Quaternion::Identity,
			TL_Math::Vector3 _worldScale = TL_Math::Vector3::One);
		static void DrawLine(TL_Math::Vector3 _loc1, TL_Math::Vector3 _loc2, float _duration = 0.0f);

		static std::vector<Line> m_VLines;

	private:
	
	



	};
}
