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

        /// @brief ���� �̸��� �´� Ŭ���� ������ ���� ��� �ִϸ��̼��� ���� ���� �ش�.
        /// @param name Ŭ�� �̸�
        void Play();

        /// @brief �ִϸ��̼��� ���߰� ��� Ÿ�ٿ�����Ʈ�� �ʱ� ���·� �ǵ��� �ش�.
        void Stop();

    protected:
        ComponentBase* CreateInstance_Impl(GameObject* _owner) override;
        ComponentBase* CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map) override;
        void CloneConvertAddress_Impl(AddressConvertMap* _map) override;

    private:
        /// @brief �ִϸ��̼��� �����ϱ����� ���ӿ�����Ʈ�� ����Ʈ
        /// �ִϸ��̼��� ����Ǿ���� ���� ������Ʈ�� �����Ǵ� ��찡 ������ ����Ǿ�� �Ѵ�.
        /// ���� ������� ���Ѵٸ� �� ������ �ִϸ��̼��� ����Ǿ���� ���ӿ�����Ʈ�� ã�ƾ� �Ѵ�.
        struct TargetList
        {
            bool bInit = false;
            std::vector<GameObject*> targets;
            std::vector<Matrix> initialLocalTMs;
        };

        void InitializeTargetList(Resource<AnimationClip>& _clip);

        /**
         * @brief ���� ������� Ŭ���� ��� ��ġ�Դϴ�.
         * �� ���� �׻� Ŭ���� ���̺��� ª���ϴ�.
        */
        float m_PlayingTime;

        /// @brief �ִϸ��̼��� ���࿩��
        bool m_IsPlaying;

        /// @brief ���� ���� ���� Ŭ�� ĳ��
        Resource<AnimationClip> m_Clip;

        /**
         * @brief ������� Ŭ���� �����ϴ� snap ��Ͽ� �����ϴ� ���� ������Ʈ ����� ������ ��ü�Դϴ�.
         * TargetList �� ���� ������Ʈ ������ snap ������ ��ġ�ؾ� �մϴ�.
        */
        TargetList m_TargetList;

    public:
        float GetPlayingTime() const { return m_PlayingTime; }

        bool GetIsPlaying() const { return m_IsPlaying; }

        Resource<AnimationClip> GetClip() const { return m_Clip; }
        void SetClip(Resource<AnimationClip> _clip) { m_Clip = _clip; }
    };
}
