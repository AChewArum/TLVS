#pragma once

#include "internal/dll.h"

#include <chrono>
#include "Common.h"

namespace TL_Application
{
    class APPLICATION_API GameTime final
    {
    public:
        static GameTime* Instance;
        GameTime();
        ~GameTime();

        void Initialize();

        void PreTick();

    private:
        std::chrono::system_clock::time_point m_StartGameTime;

        std::chrono::system_clock::time_point m_LastPreTickTime;

        std::chrono::system_clock::time_point m_CurrentPreTickTime;

    private:
        /**
         * \brief 게임이 시작된 이후 경과된 시간입니다.
         */
        long long m_GameTimeNanos;

        /**
         * \brief 지난 프레임을 처리하면서 걸린 시간입니다.
         * 이는 지난 프레임의 시작 시점과 이번 프레임의 시작 시점의 시간 차이입니다.
         */
        long long m_DeltaTimeNanos;

        /**
         * \brief 게임이 시작된 이후 지연된 시간의 총 합입니다.
         */
        long long m_ElapsedDelayedTimeNanos;

    public:
        inline static double GetGameTimeMillis() { return Instance->m_GameTimeNanos * NANOS_TO_MILLIS; }

        inline static double GetDeltaTimeMillis() { return Instance->m_DeltaTimeNanos * NANOS_TO_MILLIS; }

        inline static double GetGameTime() { return Instance->m_GameTimeNanos * NANOS_TO_SECOND; }

        inline static double GetDeltaTime() { return Instance->m_DeltaTimeNanos * NANOS_TO_SECOND; }

        inline static auto GetGameTimePoint() { return Instance->m_CurrentPreTickTime; }

    public:
        static constexpr long long SECOND_TO_MILLIS = 1'000;
        static constexpr long long SECOND_TO_MICROS = 1'000'000;
        static constexpr long long SECOND_TO_NANOS = 1'000'000'000;
        static constexpr double MILLIS_TO_SECOND = 1. / 1'000.;
        static constexpr long long MILLIS_TO_MICROS = 1'000;
        static constexpr long long MILLIS_TO_NANOS = 1'000'000;
        static constexpr double MICROS_TO_SECOND = 1. / 1'000'000.;
        static constexpr double MICROS_TO_MILLIS = 1. / 1'000.;
        static constexpr long long MICROS_TO_NANOS = 1'000;
        static constexpr double NANOS_TO_SECOND = 1. / 1'000'000'000.;
        static constexpr double NANOS_TO_MILLIS = 1. / 1'000'000.;
        static constexpr double NANOS_TO_MICROS = 1. / 1'000.;
    };
}
