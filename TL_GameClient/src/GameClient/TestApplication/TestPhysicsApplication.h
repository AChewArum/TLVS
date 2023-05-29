#pragma once

#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
    class Scene;
    class GameObject;
    class AudioClip;
}

namespace TL_Graphics
{
	class IMesh;
	class IMaterial;
	class IConstantBuffer;
	class ICamera;
}

class TestPhysicsApplication :
    public TL_GameEngine::GameApplication
{
public:
    void OnApplicationStart() override;

    void OnApplicationTick() override;

    void OnApplicationEnd() override;

private:
    TL_GameEngine::Scene* _scene;

    TL_GameEngine::AudioClip* _pTestClip;
    TL_GameEngine::GameObject* m_pCameraObject;
    TL_GameEngine::GameObject* m_pMeshObject;

	TL_Graphics::IMesh* m_Mesh;
	TL_Graphics::IMesh* m_Mesh2;
	TL_Graphics::IMesh* m_Plane;
	TL_Graphics::IMaterial* m_Material;

    TL_Graphics::IConstantBuffer* m_pColorR;
    TL_Graphics::IConstantBuffer* m_pColorG;
};
