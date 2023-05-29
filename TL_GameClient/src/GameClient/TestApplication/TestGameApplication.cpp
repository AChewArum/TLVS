#include "GameClient/TestApplication/TestGameApplication.h"

#include "GameClient/SayHelloComponent.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Scene.h"

using namespace TL_GameEngine;

void TestGameApplication::OnApplicationStart()
{
    GameApplication::OnApplicationStart();

    _scene = new Scene(TEXT("MainScene"));
    _gameObject1 = GameObject::Spawn(_scene);
    _gameObject1->SetName(TEXT("GameObject 1"));
    const auto _helloComponent = _gameObject1->AddComponent<SayHelloComponent>();
    GameWorld::GetInstance()->ReserveLoadScene(_scene);

    assert(_scene->GetName() == TEXT("MainScene"));
    assert(std::ranges::find(_scene->GetAllRootGameObjects(), _gameObject1) != _scene->GetAllRootGameObjects().end());
    assert(_gameObject1->GetName() == TEXT("GameObject 1"));
    assert(_gameObject1->GetComponent<SayHelloComponent>() == _helloComponent);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 1);
    assert(GameWorld::GetInstance()->GetComponents().size() == 1);

    _gameObject1_Child = GameObject::Spawn(_scene);
    _gameObject1_Child->SetName(TEXT("GameObject 1_Child"));
    _gameObject1_Child->AddComponent<SayHelloComponent>();

    assert(_scene->GetAllRootGameObjects().size() == 2);                                        // TEST_Scene_GetAllRootGameObjects
    assert(_scene->GetAllComponents().size() == 2);                                             // TEST_Scene_GetAllComponents
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 2);
    assert(GameWorld::GetInstance()->GetComponents().size() == 2);

    _gameObject1_Child->SetParent(_gameObject1);
    assert(_scene->GetAllRootGameObjects().size() == 1);                                        // TEST_Scene_GetAllRootGameObjectsAfterSetParent
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 1);                         // TEST_GameWorld_GetRootGameObjectsAfterSetParent
    assert(_gameObject1->GetChild(TEXT("GameObject 1_Child")) == _gameObject1_Child);           // TEST_GameObject_GetChild()
    assert(_gameObject1_Child->GetParent() == _gameObject1);                                    // TEST_GameObject_GetParent()

    /* Scene2 */

    _scene2 = new Scene(TEXT("SecondScene"));

    _gameObject2 = GameObject::Spawn(_scene2);
    _gameObject2->SetName(TEXT("GameObject 2"));
    _gameObject2_Child = GameObject::Spawn(_scene2);
    _gameObject2_Child->SetName(TEXT("GameObject 2_Child"));

    assert(_scene2->GetAllRootGameObjects().size() == 2);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 3);
    _gameObject2_Child->SetParent(_gameObject2);
    assert(_scene2->GetAllRootGameObjects().size() == 1);                                       // TEST_Scene_GetRootGameObjectsAfterSetParent
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 2);                        // TEST_Scene_GetRootGameObjectsAfterSetParent

    /* Move Object to Scene1 from Scene2 */

    _gameObject2_Child->SetScene(_scene);
    assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 3); // 3, gameObject1, gameObject2, gameObject2_Child
    assert(_scene2->GetAllRootGameObjects().size() == 1); // 1, gameObject2
    assert(_scene->GetAllRootGameObjects().size() == 2); // 2, gameObject1, gameObject2_Child
}

void TestGameApplication::OnApplicationTick()
{
    GameApplication::OnApplicationTick();

    static bool _test = false;

    if (_gameObject1_Child != nullptr)
    {
        _gameObject1_Child->ReserveDestroy();
        _gameObject1_Child = nullptr;
    }
    else if(_gameObject1 != nullptr)
    {
        assert(std::ranges::find(_scene->GetAllRootGameObjects(), _gameObject1_Child) == _scene->GetAllRootGameObjects().end());
        assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 3); // gameObject1, gameObject2, gameObject2_Child alive
        assert(GameWorld::GetInstance()->GetComponents().size() == 1); // gameObject1's HelloComponent alive

        _gameObject1->ReserveDestroy();
        _gameObject1 = nullptr;
    }
    else if(_test == false)
    {
        _test = true;

        assert(std::ranges::find(_scene->GetAllRootGameObjects(), _gameObject1) == _scene->GetAllRootGameObjects().end());
        assert(GameWorld::GetInstance()->GetRootGameObjects().size() == 2); // gameObject2, gameObject2_Child alive
        assert(GameWorld::GetInstance()->GetComponents().size() == 0);  // all dead

        GameWorld::GetInstance()->ReserveLoadScene(_scene2);
    }
    else
    {
        // gameObject2_Child alive
    }
}

void TestGameApplication::OnApplicationEnd()
{
    GameApplication::OnApplicationEnd();
}
