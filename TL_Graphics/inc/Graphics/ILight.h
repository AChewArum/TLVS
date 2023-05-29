#pragma once

#include "Export.h"

#include "Math\TL_Math.h"

namespace TL_Graphics
{
	 __declspec(align(16)) struct DirectionalLight
	{
	public:
		const float type = 0;//쉐이더에서 타입 문제 때문에 float로 사용함

	private:
		TL_Math::Vector3 position = {0,0,0};

	public:
		float intensity = 1.0f;
		TL_Math::Vector3 direction = { 0,0,1 };

	private:
		float range = 600.0f;
		TL_Math::Vector3 attenuation = { 1.0f, 0.007, 0.0002 };

		float spot = 8.0f;
	public:
		TL_Math::Vector3 color = { 1,1,1 };
	};
	__declspec(align(16)) struct PointLight
	{
	public:
		const float type = 1;//쉐이더에서 타입 문제 때문에 float로 사용함
		TL_Math::Vector3 position = { 0,0,0 };

		float intensity = 1.0f;
	private:
		TL_Math::Vector3 direction = { 0,0,1 };

	public:
		float range = 600.0f;
		TL_Math::Vector3 attenuation = { 1.0f, 0.007, 0.0002 };;

	private:
		float spot = 8.0f;
	public:
		TL_Math::Vector3 color = { 1,1,1 };
	};
	__declspec(align(16)) struct SpotLight
	{
	public:
		const float type = 2;//쉐이더에서 타입 문제 때문에 float로 사용함
		TL_Math::Vector3 position = { 0,0,0 };

		float intensity = 1.0f;
		TL_Math::Vector3 direction = { 0,0,1 };

		float range = 600.0f;
		TL_Math::Vector3 attenuation = { 1.0f, 0.007, 0.0002 };;

		float spot = 8.0f;
		TL_Math::Vector3 color = { 1,1,1 };
	};

}