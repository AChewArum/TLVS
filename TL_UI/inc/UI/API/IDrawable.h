#pragma once

namespace TL_UI
{
    class UI_API IDrawable abstract
    {
    public:
        virtual void Draw() = 0;

    protected:
        virtual ~IDrawable() = default;
    };
}
