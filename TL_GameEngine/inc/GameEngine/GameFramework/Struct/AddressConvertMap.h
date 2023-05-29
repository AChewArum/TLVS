#pragma once

#include "Common_min.h"
#include "GameEngine/internal/dll.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API AddressConvertMap
    {
    public:
        template <typename T>
        T* GetMappedTarget(void* _key)
        {
            const auto _iter = m_Map.find(_key);
            assert(_iter != m_Map.end());

            return static_cast<T*>(_iter->second);
        }

        void Mapping(void* _key, void* _target)
        {
            const auto _iter = m_Map.find(_key);
            assert(_iter == m_Map.end());

            m_Map.insert({_key, _target});
        }

    private:
        std::map<void*, void*> m_Map;
    };
}
