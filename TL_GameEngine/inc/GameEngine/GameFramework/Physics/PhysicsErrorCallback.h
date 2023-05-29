#pragma once

#include "Common\ErrorCallback.h"

namespace TL_GameEngine
{
    class PhysicsErrorCallback final : public TL_Common::ErrorCallback
    {
    public:
        PhysicsErrorCallback() = default;
        ~PhysicsErrorCallback() = default;

        virtual void Error(const char* msg) override {}

        virtual void Waring(const char* msg) override {}

        virtual void Log(const char* msg) override {}
    };
}
