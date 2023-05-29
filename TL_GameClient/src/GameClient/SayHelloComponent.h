#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

class SayHelloComponent :
    public TL_GameEngine::ComponentBase
{
public:
    SayHelloComponent(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("SayHelloComponent"))
        : ComponentBase(_gameObject, _typeName) {}

    void Tick() override;

protected:
    ComponentBase* CreateInstance_Impl(TL_GameEngine::GameObject* _owner) override
    {
        return new SayHelloComponent(_owner);
    };
};
