#include "GameEngine_pch.h"
#include "Debug.h"

#include "Application/GameTime.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/Resource/Resources.h"

namespace TL_GameEngine
{
	DEFINE_SINGLETON_CLASS(Debug, []() {}, []() {})

	std::vector<Line> Debug::m_VLines ;

	struct StaticMesh_PerObjectBuffer
	{
		TL_Math::Matrix _world;
	};

	void Debug::DrawCube(
		TL_Math::Vector3 _worldPosition,
		TL_Math::Quaternion _worldRotation,
		TL_Math::Vector3 _worldScale)
	{
		const auto _renderSystem = GameApplication::Instance->GetRenderSystem();
		auto _mesh = Resources::GetInstance()->GetDefaultCubeMesh();

		StaticMesh_PerObjectBuffer _perObjectData;
		_perObjectData._world =
			Matrix::CreateScale(_worldScale) * Matrix::CreateFromQuaternion(_worldRotation) * Matrix::CreateTranslation(_worldPosition);

		static TL_Graphics::IConstantBuffer* _worldBuffer = _renderSystem->CreateConstantBuffer(&_perObjectData, sizeof(StaticMesh_PerObjectBuffer));
		_worldBuffer->Update(&_perObjectData, sizeof(StaticMesh_PerObjectBuffer));
		_worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

		_mesh->GetMesh()->Set();
		_renderSystem->DrawWireOnce();
		_renderSystem->Draw();
	}

	void Debug::DrawLine(TL_Math::Vector3 _loc1, TL_Math::Vector3 _loc2, float _duration)
	{
		
		Line _line;
		_line.m_Line = TL_Graphics::RenderSystem::Get()->CreateLine(_loc1, _loc2);
		_line.m_Time = GameApplication::Instance->GetGameTime()->GetGameTime();
		_line.m_Duration = _duration;
		m_VLines.push_back(_line);
	}


}
