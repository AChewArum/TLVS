#include "AAlternativeRowBackgroundList.h"

#include "Common_min.h"

namespace TL_UI
{
    void AAlternativeRowBackgroundList::AddItem(std::string _item)
    {
        m_ItemList.push_back(_item);
    }

    void AAlternativeRowBackgroundList::RemoveItem(std::string _item)
    {
        const auto _iter = std::find(m_ItemList.begin(), m_ItemList.end(), _item);
        assert(_iter != m_ItemList.end());
        m_ItemList.erase(_iter);
    }

    void AAlternativeRowBackgroundList::RemoveItem(int _index)
    {
        assert(_index < m_ItemList.size());
        m_ItemList.erase(m_ItemList.begin() + _index);
    }
}
