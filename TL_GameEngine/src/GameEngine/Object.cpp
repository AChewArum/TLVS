#include "GameEngine_pch.h"
#include "GameEngine/Object.h"

#include "Common/StringHelper.h"
#include "GameEngine/Internal/UUIDGenerator.h"

namespace TL_GameEngine
{
    Object::Object(const tstring& _typeName) :
        Object(_typeName, UUIDGenerator::Generate()) { }

    Object::Object(const tstring& _typeName, const tstring& _uuid) :
        Object(
            _typeName
            , _uuid
            // , StringHelper::Format(TEXT("%s {%s}"), _typeName.c_str(), _uuid.c_str())
            , _typeName
        ) { }

    Object::Object(const tstring& _typeName, const tstring& _uuid, const tstring& _name) :
        m_TypeName(_typeName),
        m_UUID(_uuid),
        m_Name(_name) { }
}
