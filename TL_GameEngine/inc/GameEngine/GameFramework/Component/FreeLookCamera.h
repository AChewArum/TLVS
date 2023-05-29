#pragma once

#include "GameEngine/internal/dll.h"
#include "ComponentBase.h"
#include "Math/TL_Math.h"

namespace TL_GameEngine
{
    using namespace TL_Math;
    
    class GAME_ENGINE_API FreeLookCamera :
        public ComponentBase
    {
    public:
        FreeLookCamera(GameObject* _gameObject, const tstring& _typeName = TEXT("FreeLookCamera")) :
            ComponentBase(_gameObject, _typeName)
            , m_Speed(200.0f) {}

        void Tick() override;

        //안치우가 수정함
        //Tick에다가 볼때 좀 잘 안읽혀서 tick에는 기능만 돌림
        void ControlCamera();


    private:
        /**
         * @brief local space의 x축을 기준으로 회전한 이후의 기저 벡터를 얻습니다.
         * @param _angle 회전한 각도입니다. (radian)
         * @param _right right 기저 벡터입니다.
         * @param _up up 기저 벡터의 레퍼런스입니다. 함수 내에서 회전한 결과 기저 벡터로 변경됩니다.
         * @param _forward forward 기저 벡터의 레퍼런스입니다. 함수 내에서 회전한 결과 기저 벡터로 변경됩니다.
        */
        void PitchUsingMatrix(float _angle, Vector3 _right, Vector3& _up, Vector3& _forward) const;

        /**
         * @brief world space의 y축을 기준으로 회전한 이후의 기저 벡터를 얻습니다.
         * @param _angle 회전한 각도입니다. (radian)
         * @param _right right 기저 벡터의 레퍼런스입니다. 함수 내에서 회전한 결과 기저 벡터로 변경됩니다.
         * @param _up up 기저 벡터의 레퍼런스입니다. 함수 내에서 회전한 결과 기저 벡터로 변경됩니다.
         * @param _forward forward 기저 벡터의 레퍼런스입니다. 함수 내에서 회전한 결과 기저 벡터로 변경됩니다.
        */
        void RotateYUsingMatrix(float _angle, Vector3& _right, Vector3& _up, Vector3& _forward) const;

    private:
        float m_Speed;

        RTTR_ENABLE(ComponentBase)
        RTTR_REGISTRATION_FRIEND
    };
}
