#pragma once
#include <string>

#include "UI/Panel/APanel.h"

namespace TL_UI
{
    class ImGuiPanel :
        public APanel
    {
    public:
        ImGuiPanel(AUIFactory* _factory);

        ~ImGuiPanel() override = default;

        void Draw() override;

    protected:
        std::string m_Name;
        bool m_bOpen;
    };
}
