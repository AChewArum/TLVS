#pragma once

#include "Math/TL_Math.h"
#include "internal/dll.h"

namespace TL_UI
{
    struct Rect
    {
        float left;
        float top;
        float right;
        float bottom;

        inline float GetWidth() const { return right - left; }
        inline float GetHeight() const { return bottom - top; }
    };

    class UI_API RectTransform
    {
    public:
        Rect GetRect(const Rect& _parentRect) const;

        /**
         * \brief anchor는 이 ui요소를 그릴 때, 부모 사각형의 각 변으로부터 얼만큼 떨어진 사각형을 ui요소의 anchor rect로 설정할 것인지에 대한 값입니다.
         * anchor가 설정된 뒤 left, top, right, bottom값을 조절하여 ui 요소의 크기를 조절하게 됩니다.
         * 하지만 anchor의 min과 max값이 일치하는 축이 있다면, 그 축에 대해서는 left/top 또는 right/bottom 값으로 rect를 조절하지 않습니다.
         */
        TL_Math::Vector2 m_AnchorMin;
        TL_Math::Vector2 m_AnchorMax;

        /**
         * \brief left, top, right, bottom은 anchor rect의 각 변으로부터 얼만큼 떨어진 사각형을 ui요소의 rect로 확보해 사용할 것인지에 대한 값입니다.
         * 높으면 높을수록 anchor에서부터 멀리 떨어지기 때문에, 더 좁은 영역을 확보하게 됩니다.
         * 이 값은 anchor 영역을 사용하는 축에 대해서만 사용됩니다.
         */

        float m_Left;
        float m_Top;
        float m_Right;
        float m_Bottom;

        /*
         * 아래에 정의된 변수 pivot, position, size는
         * anchor의 min과 max의 값이 일치하는 축(x또는 y)에 대해서만 사용됩니다.
         * 그런 축이 없다면 ui를 그리는 데 전혀 영향을 주지 않습니다.
         */

        /**
         * \brief pivot은 ui 요소의 중심점입니다.
         */
        TL_Math::Vector2 m_Pivot;

        /**
         * \brief position은 pivot point로부터 얼만큼 떨어져있는지에 대한 값입니다.
         */
        TL_Math::Vector2 m_Position;

        /**
         * \brief size는 ui 요소의 크기입니다.
         * size를 증가시키면 pivot의 위치로부터 비례하여 양 옆으로(또는 위 아래로) 늘어납니다.
         */
        TL_Math::Vector2 m_Size;
    };

    inline Rect RectTransform::GetRect(const Rect& _parentRect) const
    {
        Rect _outRect;

        const float _parentWidth = _parentRect.GetWidth();
        const float _parentHeight = _parentRect.GetHeight();

        if (m_AnchorMin.x != m_AnchorMax.x)
        {
            // x축이 anchor 모드일 때 실행됩니다.

            const float _anchorRectLeft = _parentWidth * m_AnchorMin.x;
            const float _anchorRectRight = _parentWidth * m_AnchorMax.x;
            const float _uiRectLeft = _anchorRectLeft + m_Left;
            const float _uiRectRight = _anchorRectRight - m_Right;

            _outRect.left = _uiRectLeft;
            _outRect.right = _uiRectRight;
        }
        else
        {
            // x축이 pivot 모드일 때 실행됩니다.
            
            const float _anchorPointX = _parentWidth * m_AnchorMin.x;
            const float _fromAnchorPointToUiRectLeft = m_Size.x * m_Pivot.x;
            const float _fromAnchorPointToUiRectRight = m_Size.x * (1 - m_Pivot.x);
            const float _uiRectLeft = _anchorPointX - _fromAnchorPointToUiRectLeft;
            const float _uiRectRight = _anchorPointX + _fromAnchorPointToUiRectRight;

            _outRect.left = _uiRectLeft;
            _outRect.right = _uiRectRight;
        }

        if (m_AnchorMin.y != m_AnchorMax.y)
        {
            // y축이 anchor 모드일 때 실행됩니다.

            const float _anchorRectTop = _parentHeight * m_AnchorMin.y;
            const float _anchorRectBottom = _parentHeight * m_AnchorMax.y;
            const float _uiRectTop = _anchorRectTop + m_Top;
            const float _uiRectBottom = _anchorRectBottom - m_Bottom;

            _outRect.top = _uiRectTop;
            _outRect.bottom = _uiRectBottom;
        }
        else
        {
            // y축이 pivot 모드일 때 실행됩니다.
            
            const float _anchorPointY = _parentHeight * m_AnchorMin.y;
            const float _fromAnchorPointToUiRectTop = m_Size.y * m_Pivot.y;
            const float _fromAnchorPointToUiRectBottom = m_Size.y * (1 - m_Pivot.y);
            const float _uiRectTop = _anchorPointY - _fromAnchorPointToUiRectTop;
            const float _uiRectBottom = _anchorPointY + _fromAnchorPointToUiRectBottom;

            _outRect.top = _uiRectTop;
            _outRect.bottom = _uiRectBottom;
        }
        
        return _outRect;
    }
}
