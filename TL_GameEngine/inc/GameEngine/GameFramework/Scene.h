#pragma once

#include "GameEngine/internal/dll.h"

#include "Common.h"
#include "GameEngine/Object.h"

namespace TL_GameEngine
{
    class GameObject;
    class ComponentBase;

    class GAME_ENGINE_API Scene final :
        public Object
    {
        friend class GameWorld;
        friend class ComponentBase;
        friend class GameObject;

    public:
        Scene(const tstring& _name);

    private:
        void AddRootGameObject(GameObject* _gameObject);

        void AddComponent(ComponentBase* _component);

        void RemoveRootGameObject(GameObject* _gameObject);

        void RemoveComponent(ComponentBase* _component);

    private:
        std::vector<GameObject*> m_RootGameObjects;

        std::unordered_set<ComponentBase*> m_Components;

    public:
        inline const std::vector<GameObject*>& GetAllRootGameObjects() const { return m_RootGameObjects; }

        inline const std::unordered_set<ComponentBase*>& GetAllComponents() const { return m_Components; }
    };
}
