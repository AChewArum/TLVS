#pragma once
#include "ComponentBase.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API CameraArm
        : public ComponentBase
    {
    public:
        explicit CameraArm(
            GameObject* const _gameObject,
            const tstring& _typeName = TEXT("CameraArm")
        ) : ComponentBase(_gameObject, _typeName) {}

        void Tick() override;

    protected:
        TL_GameEngine::GameObject* m_Target;
        TL_Math::Vector3 m_Offset;

    public:
        void SetTarget(TL_GameEngine::GameObject* const _target) { m_Target = _target; }
        void SetOffset(const TL_Math::Vector3& _offset) { m_Offset = _offset; }
    };
}
