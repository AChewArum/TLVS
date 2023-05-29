#include "ATreeView.h"

namespace TL_UI
{
    void ATreeView::Draw_Impl()
    {
        for (int i = 0; i < m_Roots.size(); ++i)
        {
            auto _root{m_Roots[i]};
            DrawNodeRecursive_Impl(_root);
        }
    }
}
