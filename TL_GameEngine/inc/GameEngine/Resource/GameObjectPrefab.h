#pragma once
#include "GameEngine/GameFramework/GameObject.h"

namespace TL_GameEngine
{
    class GameObjectPrefab
    {
        friend class Resources;

    public:
        virtual ~GameObjectPrefab() = default;

        GameObjectPrefab& operator=(const GameObjectPrefab& _other) = delete;

        GameObjectPrefab& operator=(GameObjectPrefab&& _other) noexcept
        {
            if (this == &_other)
                return *this;
            m_Root = std::move(_other.m_Root);
            return *this;
        }

        GameObject* Get() const { return m_Root.get(); }

        tstring Serialize();
        static GameObjectPrefab Deserialize(const tstring& _data);

    protected:
        explicit GameObjectPrefab(GameObject* _go)
            : m_Root(_go) {}

        explicit GameObjectPrefab(GameObjectPrefab&& _other) noexcept
            : m_Root{std::move(_other.m_Root)} {}

        explicit GameObjectPrefab(const GameObjectPrefab& _other) = delete;

        std::unique_ptr<GameObject> m_Root;
    };
}
