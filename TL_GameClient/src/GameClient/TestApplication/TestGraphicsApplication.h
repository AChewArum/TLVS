#pragma once

#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
    class Scene;
    class GameObject;
    class FreeLookCamera;
}

namespace TL_Graphics
{
    class IMesh;
    class IMaterial;
    class IConstantBuffer;
    class ICamera;
}

class TestGraphicsApplication :
    public TL_GameEngine::GameApplication
{
public:
    void OnApplicationStart() override;

    void OnApplicationTick() override;

private:
    TL_GameEngine::Scene* m_Scene;
    TL_GameEngine::GameObject* m_GameObject;
    TL_GameEngine::GameObject* m_GameObject2;
    TL_GameEngine::GameObject* m_GameObject3;
    TL_GameEngine::GameObject* m_GameObject3_Child;

    TL_Graphics::IMesh* m_Mesh;
    TL_Graphics::IMaterial* m_Material;

    TL_GameEngine::GameObject* m_CameraGO;
    TL_GameEngine::FreeLookCamera* m_CameraComponent;
};
