#pragma once

namespace TL_GameEditor
{
    class InspectorView
    {
    public:
        explicit InspectorView();

        void Draw();

        void ToggleShow() { m_bOpen = !m_bOpen; }

    private:
        bool m_bOpen;
    };
}
