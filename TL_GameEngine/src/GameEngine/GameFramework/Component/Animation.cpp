#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Animation.h"

#include "Application/ApplicationContext.h"
#include "Application/GameTime.h"
#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
    Animation::Animation(class GameObject* gameObject, const tstring& type /*= TEXT("Animation")*/)
        : ComponentBase(gameObject, type)
          , m_IsPlaying(false) {}

    Animation::~Animation() { }

    void Animation::Play()
    {
        if (m_IsPlaying)
            return;

        if (m_Clip == nullptr)
            return;

        m_IsPlaying = true;
        m_PlayingTime = 0.0f;
    }

    void Animation::Stop()
    {
        if (m_IsPlaying == false)
            return;

        m_IsPlaying = false;
        m_PlayingTime = 0.0f;

        for (uint32 i = 0; i < m_Clip->GetAnimationSnapList().size(); i++)
        {
            if (m_TargetList.targets[i] == nullptr)
                return;

            m_TargetList.targets[i]->GetComponent<Transform>()->SetLocalTM(m_TargetList.initialLocalTMs[i]);
        }
    }

    ComponentBase* Animation::CreateInstance_Impl(GameObject* _owner)
    {
        return new Animation(_owner);
    }

    ComponentBase* Animation::CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map)
    {
        ComponentBase::CloneInstance_Impl(_cloned, _map);

        Animation* _anim = dynamic_cast<Animation*>(_cloned);
        _anim->m_PlayingTime = m_PlayingTime;
        _anim->m_IsPlaying = m_IsPlaying;
        _anim->m_Clip = m_Clip;
        _anim->m_TargetList = m_TargetList;

        return _anim;
    }

    void Animation::CloneConvertAddress_Impl(AddressConvertMap* _map)
    {
        ComponentBase::CloneConvertAddress_Impl(_map);

        std::for_each(m_TargetList.targets.begin(), m_TargetList.targets.end(),
            [_map](GameObject*& _target) {_target = _map->GetMappedTarget<GameObject>(_target); });
    }

    void Animation::Tick()
    {
        if (m_IsPlaying == false)
            return;

        assert(m_Clip != nullptr);

        m_PlayingTime = std::fmod(m_PlayingTime + GameApplication::Instance->GetAppContext()->GetGameTime()->GetDeltaTime(), m_Clip->GetClipLength());

        const int _snapCount = m_Clip->GetAnimationSnapList().size();
        for (uint32 i = 1; i < _snapCount; i++)
        {
            auto& _snap = m_Clip->GetAnimationSnapList()[i];

            Vector3 _pos = AnimationClip::GetLerpedVector3(_snap.m_PosKeyFrameList, m_Clip->GetClipLength(), m_PlayingTime);
            Quaternion _rot = AnimationClip::GetSlerpedQuaternion(_snap.m_RotKeyFrameList, m_Clip->GetClipLength(), m_PlayingTime);
            Vector3 _scale = AnimationClip::GetLerpedVector3(_snap.m_ScaleKeyFrameList, m_Clip->GetClipLength(), m_PlayingTime);
            Matrix _tm = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_rot) * Matrix::CreateTranslation(_pos);
            m_TargetList.targets[i]->GetComponent<Transform>()->SetLocalTM(_tm);
        }
    }

    void Animation::OnStart()
    {
        if (m_Clip != nullptr)
            InitializeTargetList(m_Clip);
    }

    void Animation::InitializeTargetList(Resource<AnimationClip>& _clip)
    {
        if (m_TargetList.bInit)
        {
            m_TargetList.targets.clear();
            m_TargetList.initialLocalTMs.clear();
        }

        const int _snapCount = _clip->GetAnimationSnapList().size();

        m_TargetList.targets.reserve(_snapCount);
        m_TargetList.bInit = true;

        for (uint32 i = 0; i < _snapCount; i++)
        {
            const auto& _snap = _clip->GetAnimationSnapList()[i];
            auto* const _findObject = GetGameObject()->FindChildRecursive(_snap.m_TargetName);

            m_TargetList.targets.emplace_back(_findObject);
            m_TargetList.initialLocalTMs.emplace_back(_findObject->GetComponent<Transform>()->GetLocalTM());
        }
    }
}
