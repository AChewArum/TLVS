#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/Component/ComponentBase.h"

#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/GameWorld.h"
#include "GameEngine/GameFramework/Scene.h"

#include "rttr/registration"
RTTR_REGISTRATION
{
    using namespace TL_GameEngine;

    rttr::registration::class_<ComponentBase>("TL_GameEngine::ComponentBase")
        // .constructor<GameObject*, const tstring&>()
        .property("m_GameObject", &ComponentBase::m_GameObject, rttr::registration::protected_access)
        .property("m_bEnable", &ComponentBase::m_bEnable, rttr::registration::protected_access)
        .property("m_ComponentFlags", &ComponentBase::m_ComponentFlags, rttr::registration::protected_access);
}

namespace TL_GameEngine
{
    ComponentBase::ComponentBase(GameObject* _gameObject, const tstring& _typeName) :
        Object(_typeName),
        m_GameObject(_gameObject),
        m_bEnable(true),
        m_Ticked(false)
    {
        m_ComponentFlags |= ComponentFlag::eDEFAULT;
    }

    void ComponentBase::PreTick()
    {
        m_Ticked = false;
    }

    void ComponentBase::Tick()
    {
        m_Ticked = true;
    }

    void ComponentBase::PostTick() { }

    void ComponentBase::FixedTick() { }

    void ComponentBase::OnAwake() { }

    void ComponentBase::OnStart() { }

    void ComponentBase::OnDestroy() {}

    void ComponentBase::OnChangeEnable(bool _newEnable) { }

    ComponentBase* ComponentBase::Clone(GameObject* _owner)
    {
        const auto _map = std::make_unique<AddressConvertMap>();
        return Clone(_owner, _map.get());
    }

    ComponentBase* ComponentBase::Clone(GameObject* _owner, AddressConvertMap* _map)
    {
        ComponentBase* _component = Clone_WithoutConvertAddress(_owner, _map);
        _component->CloneConvertAddress_Impl(_map);
        return _component;
    }

    ComponentBase* ComponentBase::Clone_WithoutConvertAddress(
        GameObject* _owner, 
        AddressConvertMap* _map)
    {
        auto _cloned = CreateInstance_Impl(_owner);
        assert(_cloned != nullptr, "복제 대상 컴포넌트 타입에서 CreateInstance_Impl을 재정의했는지 확인하세요.");
        CloneInstance_Impl(_cloned, _map);
        return _cloned;
    }

    ComponentBase* ComponentBase::CreateInstance_Impl(GameObject* _owner)
    {
        return nullptr;
    }

    ComponentBase* ComponentBase::CloneInstance_Impl(
        ComponentBase* _cloned
        , AddressConvertMap* _map
    )
    {
        _map->Mapping(this, _cloned);
        _cloned->m_GameObject = m_GameObject;
        _cloned->m_bEnable = m_bEnable;
        _cloned->m_ComponentFlags = m_ComponentFlags;
        _cloned->m_Ticked = m_Ticked;

        return _cloned;
    }

    void ComponentBase::CloneConvertAddress_Impl(AddressConvertMap* _map)
    {
        m_GameObject = _map->GetMappedTarget<GameObject>(m_GameObject);
    }
}
