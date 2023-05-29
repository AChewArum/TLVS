#pragma once
#include "GameEngine/internal/dll.h"

namespace TL_GameEngine
{
    class GameObject;

    class GAME_ENGINE_API GameObjectHelper
    {
    public:
        static void AddStaticMeshColliderRecursive(GameObject* _go);

        static void DisableStaticMeshRendererRecursive(GameObject* _go);
    };
}
