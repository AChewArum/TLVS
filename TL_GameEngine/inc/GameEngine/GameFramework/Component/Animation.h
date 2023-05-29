#pragma once

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/Resource/Resource.h"
#include "GameEngine/Resource/AnimationClip.h"

namespace TL_GameEngine
{
    class AnimationClip;

    class GAME_ENGINE_API Animation : public ComponentBase
    {
    public:
        Animation(class GameObject* gameObject, const tstring& type = TEXT("Animation"));
        ~Animation() override;

        //////////////////////////////////////////////////////////////////////////
        // Inherited Component
        //////////////////////////////////////////////////////////////////////////

        void Tick() override;

        void OnStart() override;

        //////////////////////////////////////////////////////////////////////////
        // Member Function
        //////////////////////////////////////////////////////////////////////////

        /// @brief 들어온 이름에 맞는 클립을 가지고 있을 경우 애니메이션을 실행 시켜 준다.
        /// @param name 클립 이름
        void Play();

        /// @brief 애니메이션을 멈추고 모든 타겟오브젝트를 초기 상태로 되돌려 준다.
        void Stop();

    protected:
        ComponentBase* CreateInstance_Impl(GameObject* _owner) override;
        ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;
        void CloneConvertAddress_Impl(AddressConvertMap* _map) override;

    private:
        /// @brief 애니메이션을 적용하기위한 게임오브젝트의 리스트
        /// 애니메이션이 적용되어야할 게임 오브젝트가 삭제되는 경우가 없음이 보장되어야 한다.
        /// 만약 보장되지 못한다면 매 프레임 애니메이션이 적용되어야할 게임오브젝트를 찾아야 한다.
        struct TargetList
        {
            bool bInit = false;
            std::vector<GameObject*> targets;
            std::vector<Matrix> initialLocalTMs;
        };

        void InitializeTargetList(Resource<AnimationClip>& _clip);

        /**
         * @brief 현재 재생중인 클립의 재생 위치입니다.
         * 이 값은 항상 클립의 길이보다 짧습니다.
        */
        float m_PlayingTime;

        /// @brief 애니메이션이 실행여부
        bool m_IsPlaying;

        /// @brief 현재 적용 중인 클립 캐싱
        Resource<AnimationClip> m_Clip;

        /**
         * @brief 재생중인 클립이 포함하는 snap 목록에 대응하는 게임 오브젝트 목록을 가지는 객체입니다.
         * TargetList 내 게임 오브젝트 개수는 snap 개수와 일치해야 합니다.
        */
        TargetList m_TargetList;

    public:
        float GetPlayingTime() const { return m_PlayingTime; }

        bool GetIsPlaying() const { return m_IsPlaying; }

        Resource<AnimationClip> GetClip() const { return m_Clip; }
        void SetClip(Resource<AnimationClip> _clip) { m_Clip = _clip; }
    };
}
