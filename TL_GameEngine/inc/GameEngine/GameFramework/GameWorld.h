#pragma once

#include "GameEngine/internal/dll.h"

#include "Scene.h"
#include "Common.h"

namespace TL_GameEngine
{
    class Camera;
    class Scene;
    class GameObject;
    class ComponentBase;
    class IRenderableComponent;

    class GAME_ENGINE_API GameWorld final
    {
        DECLARE_SINGLETON_CLASS(GameWorld)
        friend class Scene;
        friend class GameObject;
        friend class ComponentBase;

    public:
        GameWorld(const GameWorld& _other) = delete;
        GameWorld(GameWorld&& _other) noexcept = delete;
        GameWorld& operator=(const GameWorld& _other) = delete;
        GameWorld& operator=(GameWorld&& _other) noexcept = delete;

        void Initialize();
        void Release();

        void ReserveLoadScene(Scene* _scene);

        // void ReserveLoadScene(const tstring& _sceneName);

        void OnStart();
        void Tick();

        void FixedTick();

        void Render();

        /**
         * \brief 오브젝트 삭제를 예약합니다.
         * 게임 오브젝트를 삭제할 때, 반드시 GameObject 객체를 직접 delete하지 않고 이 함수를 대신 사용해야 합니다.
         */
        void ReserveDestroy(GameObject* _gameObject);

        /**
         * \brief 컴포넌트 삭제를 예약합니다.
         * 컴포넌트를 삭제할 때, 반드시 Component 객체를 직접 delete하지 않고 이 함수를 대신 사용해야 합니다.
         */
        void ReserveDestroy(ComponentBase* _component);

        /**
         * @brief 이름에 해당하는 레이어의 인덱스 값을 반환 합니다.
         * @param _layerName 인덱스 값을 찾아야하는 레이어의 이름입니다.
         * @return 해당 레이어의 인덱스 값입니다. 찾지 못했을 경우 -1을 반환합니다.
         */
        uint32 FindLayerIndex(const tstring& _layerName);

        /**
        * @brief 인덱스에 해당하는 레이어의 이름을 반환 합니다.
        * @param _layer 찾을 레이어의 인덱스 값입니다. 최대값은 31입니다.
        * @return 해당 레이어의 이름입니다. 찾지 못했을 경우 0을 반환합니다.
        */
        tstring& FindLayerName(uint32 _layer);

        int FindGameObjectTag(const tstring& _tagName);
        int AddGameObjectTag(const tstring& _tagName);

    private:
        /**
         * @brief 파괴가 예약된 오브젝트들의 파괴를 수행합니다.
         */
        void ProceedDestroy();

        void DestroyImmediate(Scene* _scene);

        /**
         * @breif 지난 프레임에 추가된 컴포넌트들의 Start를 수행합니다.
         */
        void ProceedOnAwakeAndStart();

        void ProceedLoadScene();

    private:
        /* Callbacks */

        /**
         * @brief (이 메소드는 GameObject::AddComponent에서만 호출됩니다.)
         * 이 Component가 작동되기 위해 월드에 등록합니다.
         * @param _scene 컴포넌트가 새로이 포함될 대상 씬입니다.
         * @param _gameObject 대상 컴포넌트입니다.
        */
        void AddComponent(ComponentBase* _component);

        /**
         * @brief (이 메소드는 GameWorld::DestroyImmediate(ComponentBase* _component)에서만 호출됩니다.)
         * 이 컴포넌트를 더 이상 작동하지 않도록 GameWorld에서 등록을 해제합니다.
         * @param _component 제외 대상인 컴포넌트입니다.
         */
        void RemoveComponent(ComponentBase* _component);

        /**
         * @brief 이 게임 오브젝트를 루트로 등록합니다.
         * @param _handle 
        */
        void PushUniqueHandleAsRoot(std::unique_ptr<GameObject>&& _handle);

        /**
         * @brief 이 게임 오브젝트의 unqiue_ptr을 돌려줍니다.
         * @param unique_ptr을 받기 원하는 게임 오브젝트입니다. 반드시 world상에 루트 게임 오브젝트로 존재하는 게임 오브젝트여야 합니다.
        */
        std::unique_ptr<GameObject> PopUniqueHandleOf(GameObject* _go);

    private:
        std::vector<std::unique_ptr<GameObject>> m_RootGameObjects;

        std::vector<GameObject*> m_WaitingForDestroyGameObjects;

        std::vector<ComponentBase*> m_WaitingForDestroyComponents;

        std::vector<ComponentBase*> m_WaitingForAwakeComponents;

        std::vector<ComponentBase*> m_Components;

        std::vector<IRenderableComponent*> m_RenderableComponents;

        std::vector<tstring> m_Layers;

        std::map<tstring, GameObjectTag> m_TagMap;

        Scene* m_ActiveScene;

        Scene* m_LoadSceneReserved;

        Camera* m_MainCamera;

    public:
        static Scene* GetActiveScene() { return Instance->m_ActiveScene; }

        Camera* GetMainCamera() { return m_MainCamera; }
        void SetMainCamera(Camera* _camera) { m_MainCamera = _camera; }

        std::vector<GameObject*> GetRootGameObjects() const
        {
            std::vector<GameObject*> _out{m_RootGameObjects.size()};
            for (int i = 0; i < m_RootGameObjects.size(); ++i)
                _out[i] = m_RootGameObjects[i].get();
            return _out;
        }

        auto& GetComponents() { return m_Components; }
    };
}
