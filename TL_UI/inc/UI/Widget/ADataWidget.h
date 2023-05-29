#pragma once
#include "AWidget.h"

namespace TL_UI
{
    template <typename T>
    class ADataWidget :
        public AWidget
    {
    public:
        explicit ADataWidget() = default;
        explicit ADataWidget(T _data) : m_Data(_data) {}
        explicit ADataWidget(T&& _data) : m_Data(_data) {}

        void SetData(T _data) { m_Data = _data; }
        void SetData(T&& _data) { m_Data = _data; }

    protected:
        T m_Data;
    };
}
