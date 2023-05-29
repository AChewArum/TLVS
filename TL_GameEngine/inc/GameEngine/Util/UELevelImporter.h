#pragma once

#include "GameEngine/internal/dll.h"

namespace TL_GameEngine {
    class Scene;

    class GAME_ENGINE_API UELevelImporter
    {
    public:
        static void Import(Scene* _scene, const tstring& _path);

    };
}
