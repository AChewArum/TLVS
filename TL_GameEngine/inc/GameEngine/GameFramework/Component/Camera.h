#pragma once
#include "ComponentBase.h"

namespace TL_Graphics
{
    class ICamera;
}

namespace TL_GameEngine
{
    class GAME_ENGINE_API Camera :
        public ComponentBase
    {
    public:
        Camera(GameObject* const _gameObject, const tstring& _typeName = TEXT("Camera")) :
            ComponentBase(_gameObject, _typeName)
            , m_Camera(nullptr) {}

        void OnStart() override;

        void OnDestroy() override;

        void PostTick() override;

    private:
        TL_Graphics::ICamera* m_Camera;
    };
}
