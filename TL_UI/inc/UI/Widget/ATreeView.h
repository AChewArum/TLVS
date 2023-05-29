#pragma once
#include <functional>
#include <string>
#include <algorithm>
#include <iterator>

#include "AWidget.h"
#include "UI/AWidgetContainer.h"

namespace TL_UI
{
    class ATreeView
        : public AWidget
    {
    public:
        using Target = void;

        using GetLabelStringFunc = std::function<std::string(Target*)>;
        using GetChildListFunc = std::function<std::vector<Target*>(Target*)>;
        using OnDropFunc = std::function<void(Target*, Target*)>;

    protected:
        virtual void DrawNodeRecursive_Impl(Target* _data) = 0;

        void Draw_Impl() final;

        const auto& GetRoots() const { return m_Roots; }
        std::string GetLabel(Target* _obj) const { return m_GetLabelStringFunc(_obj); }
        std::vector<Target*> GetChildList(Target* _obj) const { return m_GetChildListFunc(_obj); }
        void OnDrop(Target* _origin, Target* _other) const { m_OnDropFunc(_origin, _other); }

    private:
        GetLabelStringFunc m_GetLabelStringFunc;
        OnDropFunc m_OnDropFunc;
        GetChildListFunc m_GetChildListFunc;
        std::vector<void*> m_Roots;

    public:
        template <typename T>
        void SetRoots(const std::vector<T*> _roots)
        {
            m_Roots.clear();
            std::transform(
                _roots.begin(), _roots.end(),
                std::back_inserter(m_Roots),
                [](T* _ptr) { return static_cast<void*>(_ptr); });
        }

        void SetGetLabelStringFunc(const GetLabelStringFunc& _func) { m_GetLabelStringFunc = _func; }
        void SetOnDropFunc(const OnDropFunc& _func) { m_OnDropFunc = _func; }
        void SetGetChildListFunc(const GetChildListFunc& _func) { m_GetChildListFunc = _func; }
    };
}
