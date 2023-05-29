#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/Camera.h"

#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/GameFramework/GameWorld.h"

namespace TL_GameEngine
{
    void Camera::OnStart()
    {
        ComponentBase::OnStart();

        m_Camera = TL_Graphics::RenderSystem::Get()->CreateCamera();
        m_Camera->Set();

        if (GetGameObject()->GetTag() == GameWorld::GetInstance()->FindGameObjectTag(TEXT("MainCamera")))
        {
            Camera* _worldMainCamera = GameWorld::GetInstance()->GetMainCamera();

            // 이 카메라를 메인 카메라로 등록할 때
            // 이 카메라가 유일한 메인 카메라인지 보장받아야 합니다.
            assert(_worldMainCamera == nullptr);

            GameWorld::GetInstance()->SetMainCamera(this);
        }
    }

    void Camera::OnDestroy()
    {
        ComponentBase::OnDestroy();

        GameApplication::Instance->GetRenderSystem()->Return(m_Camera);

        if (GameWorld::GetInstance()->GetMainCamera() == this)
            GameWorld::GetInstance()->SetMainCamera(nullptr);
    }

    void Camera::PostTick()
    {
        ComponentBase::PostTick();

        m_Camera->Update(GetGameObject()->GetComponent<Transform>()->GetWorldTM());
        m_Camera->Set();
        m_Camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);
    }
}
