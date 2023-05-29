#pragma once

#include "GameEngine/internal/dll.h"

#include "Common.h"
#include "GameEngine/Object.h"
#include "GameEngine/GameFramework/Struct/AddressConvertMap.h"

#include "rttr/type"
#include "rttr/registration_friend"

namespace TL_GameEngine
{
    class GameObject;

    /*
    * Todo : 다중 상속을 처리하기 위해서 만든 임시 Flags
    * Author	: LWT
    * Date		: 2022. 11. 07
    */
    struct ComponentFlag
    {
        enum Enum
        {
            eDEFAULT = (1 << 0),
            eRENDER = (1 << 1),
            eTRIGGER = (1 << 2),
            eCOLLISION = (1 << 3),

            eTRIGGER_DEFAULT = eDEFAULT | eTRIGGER,
            eCOLLISION_DEFAULT = eDEFAULT | eCOLLISION
        };
    };

    using ComponentFlags = uint32;

    class GAME_ENGINE_API ComponentBase abstract :
        public Object
    {
    public:
        ComponentBase(GameObject* _gameObject, const tstring& _typeName);

        virtual ~ComponentBase() = default;

        virtual void PreTick();

        virtual void Tick();

        virtual void PostTick();

        virtual void FixedTick();

        virtual void OnAwake();

        virtual void OnStart();

        virtual void OnDestroy();

        virtual void OnChangeEnable(bool _newEnable);

        inline bool GetEnable() { return m_bEnable; }

        ComponentBase* Clone(GameObject* _owner);

    protected:
        ComponentBase* Clone(
            GameObject* _owner
            , AddressConvertMap* _map
        );

        ComponentBase* Clone_WithoutConvertAddress(
            GameObject* _owner,
            AddressConvertMap* _map
        );

        /**
         * \brief 이 타입의 인스턴스를 새로 생성해 반환하는 함수입니다.
         * 컴포넌트 제작자가 컴포넌트 클래스마다 새로 재정의해주어야 합니다.
         */
        virtual ComponentBase* CreateInstance_Impl(
            GameObject* _owner
        );

        /**
         * \brief 클론으로 생성된 인스턴스에 값을 채워넣는 방법을 정의한 함수입니다.
         * 컴포넌트 제작자가 컴포넌트 클래스마다 수동으로 재정의해주어야 합니다.
         * @note 포인터 타입의 경우, 이 함수에서 직접 복사해주지 않고 단순히 주소값만을 복사해주어야 합니다.
         * 이 클론에 모든 값이 채워진 이후, CloneConvertAddress_Impl 함수를 호출해 주소값을 실제로 클론하여 생성된 인스턴스의 주소로 변환합니다.
         * @param _cloned 클론된 컴포넌트 인스턴스입니다.
         */
        virtual ComponentBase* CloneInstance_Impl(
            ComponentBase* _cloned
            , AddressConvertMap* _map
        );

        /**
         * @brief 클론으로 생성된 인스턴스 내 포인터 타입이 저장한 주소값을 실제로 클론하여 생성된 인스턴스의 주소로 변환합니다.
         * 이 함수는 CloneInstance_Impl 함수에 의해 클론에 모든 값이 기록된 이후 호출됩니다.
         * @param _map 
        */
        virtual void CloneConvertAddress_Impl(
            AddressConvertMap* _map
        );

        GameObject* m_GameObject;

        /// <summary>
        /// 컴포넌트의 활성화 여부입니다.
        ///	비활성화된 컴포넌트는 Tick이 호출되지 않습니다.
        /// </summary>
        bool m_bEnable;

        /*
        * Todo : 다중 상속을 처리하기 위해서 만든 임시 Flags
        * Author	: LWT
        * Date		: 2022. 11. 07
        */
        uint32 m_ComponentFlags = ComponentFlag::eDEFAULT;

        /// <summary>
        /// 이번 프레임에 Tick이 실행되었는지에 대한 여부입니다.
        ///	매 프레임의 처리를 하기 전에 OnPreTick()이 호출되어 이 플래그를 false로 초기화해줍니다.
        /// </summary>
        bool m_Ticked;

    public:
        inline GameObject* GetGameObject() const { return m_GameObject; }

        inline void SetEnable(bool _value)
        {
            if (_value == m_bEnable)
                return;

            m_bEnable = _value;
            OnChangeEnable(_value);
        }

        /*
        * Todo : 다중 상속을 처리하기 위해서 만든 임시 Flags
        * Author	: LWT
        * Date		: 2022. 11. 07
        */
        inline void AddFlag(ComponentFlag::Enum flag)
        {
            m_ComponentFlags |= flag;
        }

        inline bool CheckFlag(ComponentFlag::Enum flag)
        {
            return (m_ComponentFlags & flag) != 0;
        }

    private:
        friend class GameObject;

        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
    };

    class GAME_ENGINE_API IRenderableComponent abstract
    {
    public:
        IRenderableComponent(ComponentBase* _componentBase)
        {
            _componentBase->AddFlag(ComponentFlag::eRENDER);
        }

        virtual void Render() abstract;
    };
}
