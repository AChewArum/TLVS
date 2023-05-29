#pragma once

#include "GameEngine/Object.h"

#include "AnimationClip.h"
#include "GameObjectPrefab.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "AnimatorController.h"

namespace TL_GameEngine
{
    template <typename TRes>
    class Resource :
        public Object
    {
        friend class Resources;

        class TResRedirect
        {
            TRes* m_pRes;

        public:
            TResRedirect() : m_pRes(nullptr) {}

            ~TResRedirect()
            {
                delete m_pRes;
                m_pRes = nullptr;
            }

            TRes* GetPtr() const { return m_pRes; };
            void SetPtr(TRes* _res) { m_pRes = _res; }
        };

    public:
        explicit Resource(const tstring& _typeName = TEXT("Resource<TRes>")) :
            Object(_typeName)
            , m_pResRedirect(std::make_shared<TResRedirect>()) { }

        Resource(const Resource& _other) :
            Object{_other}
            , m_OnLoad{_other.m_OnLoad}
            , m_pResRedirect{_other.m_pResRedirect} { }

        Resource& operator=(const Resource& _other)
        {
            if (this == &_other)
                return *this;
            Object::operator =(_other);
            m_OnLoad = _other.m_OnLoad;
            m_pResRedirect = _other.m_pResRedirect;

            return *this;
        }

        Resource& operator=(Resource&& _other) noexcept
        {
            if (this == &_other)
                return *this;
            Object::operator =(std::move(_other));
            m_OnLoad = std::move(_other.m_OnLoad);
            m_pResRedirect = _other.m_pResRedirect;

            return *this;
        }
        
        bool operator==(const Resource<TRes>& _other)
        {
            return GetPtr() == _other.GetPtr();
        }

        bool operator!=(const Resource<TRes>& _other)
        {
            return GetPtr() != _other.GetPtr();
        }

		bool operator==(nullptr_t)
		{
			return m_pResRedirect.get() == nullptr;
		}

		bool operator!=(nullptr_t)
		{
			return m_pResRedirect->GetPtr() != nullptr;
		}

        // void Load_Impl();
        //
        // void Unload_Impl();
        //
        // void Load()
        // {
        //     if (m_pResRedirect != nullptr)
        //         Unload_Impl();
        //
        //     Load_Impl();
        //     m_OnLoad.Invoke(m_pResRedirect);
        // }

    private:
        std::shared_ptr<TResRedirect> m_pResRedirect;
        TL_Common::Event<Resource<TRes>> m_OnLoad;

    public:
        TRes* GetPtr() const { return m_pResRedirect.get()->GetPtr(); }
        void SetPtr(TRes* _res) { m_pResRedirect.get()->SetPtr(_res); }
        bool IsLoaded() const { return GetPtr() != nullptr; }

        TRes* operator->() { return GetPtr(); }
    };
}
