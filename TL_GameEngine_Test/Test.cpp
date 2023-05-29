#include "gtest/gtest.h"

#include "Common_min.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/ComponentBase.h"

using namespace TL_GameEngine;

class GameEngineTest : public ::testing::Test
{
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    GameEngineTest()
    {
        // You can do set-up work for each test here.
    }

    ~GameEngineTest() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

class TestComponent :
    public ComponentBase
{
public:
    TestComponent(GameObject* _gameObject, const tstring& _typeName = TEXT("TestComponent"))
        : ComponentBase(_gameObject, _typeName) {}
};

TEST(GameEngineTest, Object_Name)
{
    GameApplication* app = new GameApplication();
    app->Start(nullptr);
    {
        Scene* _scene = new Scene(TEXT("MainScene"));
        GameObject* _gameObject1 = GameObject::Spawn(_scene);
        _gameObject1->SetName(TEXT("GameObject 1"));
        TestComponent* _component1 = _gameObject1->AddComponent<TestComponent>();
        _component1->SetName(TEXT("Component 1"));

        GameWorld::GetInstance()->ReserveLoadScene(_scene);
        app->Tick();

        ASSERT_EQ(_scene->GetName(), TEXT("MainScene"));
        ASSERT_EQ(_gameObject1->GetName(), TEXT("GameObject 1"));
        ASSERT_EQ(_component1->GetName(), TEXT("Component 1"));
    }
    app->End();
    delete app;
    app = nullptr;
}

TEST(GameEngineTest, GameWorld_ReserveLoadScene)
{
    GameApplication* app = new GameApplication();
    app->Start(nullptr);
    {
        Scene* _scene = new Scene(TEXT("MainScene"));
        Scene* _scene2 = new Scene(TEXT("SecondScene"));

        // 씬 로드를 예약하면 한 Tick 뒤에 예약한 씬이 로드됩니다.
        GameWorld::GetInstance()->ReserveLoadScene(_scene);
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetActiveScene(), _scene);

        GameWorld::GetInstance()->ReserveLoadScene(_scene2);
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetActiveScene(), _scene2);
    }
    app->End();
    delete app;
    app = nullptr;
}

TEST(GameEngineTest, GameWorld_RootGameObjects)
{
    GameApplication* app = new GameApplication();
    app->Start(nullptr);
    {
        Scene* _scene = new Scene(TEXT("MainScene"));
        GameWorld::GetInstance()->ReserveLoadScene(_scene);
        app->Tick();

        GameObject* _gameObject1 = GameObject::Spawn(_scene);
        _gameObject1->SetName(TEXT("GameObject 1"));
        ASSERT_EQ(GameWorld::GetInstance()->GetRootGameObjects().size(), 1);

        GameObject* _gameObject2 = GameObject::Spawn(_scene);
        _gameObject2->SetName(TEXT("GameObject 2"));
        ASSERT_EQ(GameWorld::GetInstance()->GetRootGameObjects().size(), 2);

        // RootGameObjects는 게임 월드의 최상위 오브젝트 목록입니다.
        // RootGameObject가 부모를 갖게 되면 더 이상 RootGameObject가 아니므로, 목록에서 제외되어야 합니다.
        GameObject* _gameObject2_Child = GameObject::Spawn(_scene);
        _gameObject2_Child->SetName(TEXT("GameObject 2_Child"));
        ASSERT_EQ(GameWorld::GetInstance()->GetRootGameObjects().size(), 3);
        ASSERT_EQ(_scene->GetAllRootGameObjects().size(), 3);

        _gameObject2_Child->SetParent(_gameObject2);
        ASSERT_EQ(GameWorld::GetInstance()->GetRootGameObjects().size(), 2);
        ASSERT_EQ(_scene->GetAllRootGameObjects().size(), 2);

        // RootGameObject가 파괴되면, 목록에서 제외되어야 합니다.
        _gameObject1->ReserveDestroy();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetRootGameObjects().size(), 1);
        ASSERT_EQ(_scene->GetAllRootGameObjects().size(), 1);
    }
    app->End();
    delete app;
    app = nullptr;
}

TEST(GameEngineTest, GameWorld_Components)
{
    GameApplication* app = new GameApplication();
    app->Start(nullptr);
    {
        Scene* _scene = new Scene(TEXT("MainScene"));
        GameWorld::GetInstance()->ReserveLoadScene(_scene);
        app->Tick();

        GameObject* _gameObject1 = GameObject::Spawn(_scene);
        _gameObject1->SetName(TEXT("GameObject 1"));
        GameObject* _gameObject2 = GameObject::Spawn(_scene);
        _gameObject2->SetName(TEXT("GameObject 2"));
        GameObject* _gameObject3 = GameObject::Spawn(_scene);
        _gameObject3->SetName(TEXT("GameObject 3"));

        // 게임 오브젝트에 컴포넌트가 부착되면, GameWorld의 Components 목록에 추가됩니다.
        const auto _component = _gameObject1->AddComponent<TestComponent>();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 1);

        _gameObject2->AddComponent<TestComponent>();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 2);

        _gameObject3->AddComponent<TestComponent>();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 3);

        // 컴포넌트를 삭제하면, GameWorld의 Components 목록에서도 제외되어야 합니다.
        _gameObject2->RemoveComponent<TestComponent>();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 2);

        _gameObject1->RemoveComponent(_component);
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 1);

        // 게임 오브젝트가 파괴되면, 게임 오브젝트에 부착된 모든 컴포넌트도 파괴되어야 합니다.
        _gameObject3->ReserveDestroy();
        app->Tick();
        ASSERT_EQ(GameWorld::GetInstance()->GetComponents().size(), 0);
    }
    app->End();
    delete app;
    app = nullptr;
}
