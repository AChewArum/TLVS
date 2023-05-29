#pragma once

#include "GameEngine/internal/dll.h"

#include "Common_min.h"

namespace TL_GameEngine
{
    class GAME_ENGINE_API Object
    {
    public:
        Object(const tstring& _typeName);

        Object(const tstring& _typeName, const tstring& _uuid);

        Object(const tstring& _typeName, const tstring& _uuid, const tstring& _name);

        tstring m_Name;

        uuid m_UUID;

        tstring m_TypeName;

        [[nodiscard]] tstring GetName() const { return m_Name; }

        [[nodiscard]] uuid GetUUID() const { return m_UUID; }

        [[nodiscard]] tstring GetTypeName() const { return m_TypeName; }

        void SetName(const tstring& _name) { m_Name = _name; }

        void SetUUID(const tstring& _uuid) { m_UUID = _uuid; }
    };
}
