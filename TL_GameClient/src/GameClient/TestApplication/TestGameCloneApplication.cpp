#include "TestGameCloneApplication.h"

#include "GameClient/SayHelloComponent.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"

using namespace TL_GameEngine;

void TestGameCloneApplication::OnApplicationStart()
{
    GameApplication::OnApplicationStart();

    _scene = new Scene(TEXT("MainScene"));
    GameWorld::GetInstance()->ReserveLoadScene(_scene);

    _gameObject1 = GameObject::Spawn(_scene);
    _gameObject1->SetName(TEXT("GameObject 1"));

    _gameObject2 = GameObject::Spawn(_scene);
    _gameObject2->SetName(TEXT("GameObject 2"));
    _gameObject2_Child = GameObject::Spawn(_scene);
    _gameObject2_Child->SetName(TEXT("GameObject 2_Child"));
    _gameObject2_Child->SetParent(_gameObject2);

    _gameObject3 = GameObject::Spawn(_scene);
    _gameObject3->SetName(TEXT("GameObject 3"));
    _gameObject3->AddComponent<SayHelloComponent>();
    _gameObject3_Child = GameObject::Spawn(_scene);
    _gameObject3_Child->SetName(TEXT("GameObject 3_Child"));
    _gameObject3_Child->SetParent(_gameObject3);
    _gameObject3_Child->AddComponent<SayHelloComponent>();

    assert(_scene->GetAllRootGameObjects().size() == 3);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 3);
    assert(GameWorld::GetInstance()->GetComponents().size() == 2);

    /* _gameObject1 Clone */

    _gameObject1_Cloned = _gameObject1->Clone();
    assert(_scene->GetAllRootGameObjects().size() == 4);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 4);
    assert(_gameObject1_Cloned->GetChilds().size() == 0);

    /* _gameObject2 Clone */

    _gameObject2_Cloned = _gameObject2->Clone();
    assert(_scene->GetAllRootGameObjects().size() == 5);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 5);
    assert(_gameObject2_Cloned->GetChilds().size() == 1);

    /* _gameObject3 Clone */

    _gameObject3_Cloned = _gameObject3->Clone();
    assert(_scene->GetAllRootGameObjects().size() == 6);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 6);
    assert(GameWorld::GetInstance()->GetComponents().size() == 4);
    assert(_gameObject3_Cloned->GetAllComponents().size() == 1);
    assert(_gameObject3_Cloned->GetComponent<SayHelloComponent>() != nullptr);
    assert(_gameObject3_Cloned->GetChilds().size() == 1);
    assert(_gameObject3_Cloned->GetChild(0)->GetAllComponents().size() == 1);
    assert(_gameObject3_Cloned->GetChild(0)->GetComponent<SayHelloComponent>() != nullptr);
}
