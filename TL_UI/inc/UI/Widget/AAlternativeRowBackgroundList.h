#pragma once

#include "string"
#include "vector"

#include "AWidget.h"

namespace TL_UI
{
    class UI_API AAlternativeRowBackgroundList :
        public AWidget
    {
    public:
        void AddItem(std::string _item);

        void RemoveItem(std::string _item);

        void RemoveItem(int _index);

    protected:
        std::vector<std::string> m_ItemList;
    };
}
