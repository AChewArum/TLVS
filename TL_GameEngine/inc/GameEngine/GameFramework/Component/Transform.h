#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Math/TL_Math.h"

namespace TL_GameEngine
{
    using namespace TL_Math;
    class GAME_ENGINE_API Transform : public ComponentBase
    {
    public:
        Transform(class GameObject* _gameObject, const tstring& _typeName = TEXT("Transform"));

    protected:
        ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;

     private:
         void UpdateWorldMatrix(const Matrix& _parentTM);

         /**
          * \brief 로컬 변환 행렬입니다.
          */
         Matrix m_LocalTM;
    
         /**
          * \brief 월드 변환 행렬입니다.
          */
         Matrix m_WorldTM;
    
         /**
          * \brief 가장 마지막으로 월드 변환 행렬이 계산된 이후
          * 값이 한 번이라도 수정되어서 변경될 필요가 있는지에 대한 판별값입니다.
          * 이 변수는 행렬 계산을 최소한으로 줄이기 위해 사용됩니다.
          */
         bool m_bWorldTMShouldUpdate;
    
     public:

         /**
          * \brief 부모를 변환시 현재 로컬 트랜스폼을 새로 적용된 부모의 
          * 월드 트랜스폼 기준으로 갱신합니다.
          */
         void ChangeParent(Transform* _parent);

         void SetLocalPosition(const Vector3& _newPosition);
    
         void SetLocalRotation(const Quaternion& _newRotation);
    
         void SetLocalScale(const Vector3& _newScale);

         void SetWorldPosition(const Vector3& _newPosition);
    
         void SetWorldRotation(const Quaternion& _newRotation);
    
         void SetWorldScale(const Vector3& _newScale);



         /*
          * Add for physics update
          * Author	: LWT
          * Date		: 2022. 10. 31
          */
         void SetLocalPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation);
         void SetPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation);

         inline Vector3 GetLocalPosition()
         {
             Vector3 _scale;
             Quaternion _rotation;
             Vector3 _position;
    
             Matrix _localTM = GetLocalTM();
             _localTM.Decompose(_scale, _rotation, _position);
    
             return _position;
         }
    
         inline Quaternion GetLocalRotation()
         {
             Vector3 _scale;
             Quaternion _rotation;
             Vector3 _position;
    
             Matrix _localTM = GetLocalTM();
             _localTM.Decompose(_scale, _rotation, _position);
    
             return _rotation;
         }
    
         inline Vector3 GetLocalScale()
         {
             Vector3 _scale;
             Quaternion _rotation;
             Vector3 _position;
    
             Matrix _localTM = GetLocalTM();
             _localTM.Decompose(_scale, _rotation, _position);
    
             return _scale;
         }
    
         Vector3 GetWorldPosition();
    
         Quaternion GetWorldRotation();
    
         Vector3 GetWorldScale();
    
         void SetLocalTM(const Matrix& _localTM);
    
         void SetWorldTM(const Matrix& _worldTM);
    
         Matrix GetLocalTM() const;
    
         Matrix GetWorldTM();
    
         Vector3 GetRightDirection();
    
         Vector3 GetUpDirection();
    
         Vector3 GetForwardDirection();

     private:
         void TryUpdateWorldTM();
    
         void MarkWorldTMDirt();
    
     private:
         static constexpr float RAD_TO_DEG = 57.295779513f;
    
         static constexpr float DEG_TO_RAD = 0.01745329252f;

         friend class GameObject;
    };
}
