#pragma once
#include "GameEngine/GameApplication.h"
#include "GameEngine/Resource/Resource.h"

namespace TL_Graphics
{
    class ICamera;
}

namespace TL_GameEngine
{
    class Camera;
    class Scene;
    class GameObject;
}

class TestFbxGraphicsApplication :
    public TL_GameEngine::GameApplication
{
public:
    void OnApplicationStart() override;

    void OnApplicationTick() override;

    void OnApplicationEnd() override;

private:
    TL_GameEngine::Scene* m_Scene;
    TL_GameEngine::GameObject* m_CameraGO;

    TL_GameEngine::Resource<TL_GameEngine::StaticMesh> m_FbxMeshRes;
    TL_GameEngine::GameObject* m_FbxMeshGO;
    TL_GameEngine::GameObject* m_FbxAnimGO;

    TL_GameEngine::GameObject* m_DirectionalLightGO;
};
