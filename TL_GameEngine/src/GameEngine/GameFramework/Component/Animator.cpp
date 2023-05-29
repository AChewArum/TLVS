#include "GameEngine_pch.h"

#include "GameEngine/Gameframework/Component/Animator.h"

#include "GameEngine/GameFramework/Component/Transform.h"
#include "GameEngine/GameFramework/GameObject.h"

#include "Application/ApplicationContext.h"
#include "Application/GameTime.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/Debug.h"

#include "Log/Log.h"

namespace TL_GameEngine
{
	Animator::Animator(class GameObject* _gameObject, const tstring& _typeName /*= TEXT("Animator")*/)
		: ComponentBase(_gameObject, _typeName), m_PlayingTime(0.f), m_PrePlayingTime(0.f), m_IsPlaying(false), m_CurrIndex(-1), m_IsTrasition(false), m_Duration(0.5f) { }

	Animator::~Animator() { }

	void Animator::PreTick()
	{
		__super::PreTick();
	}

	bool Animator::AnimatorTick()
	{
		if (m_IsPlaying == false)
			return true;

		assert(m_CurrAnimationClip != nullptr);

		// TODO: GameTime::GetDeltaTime();
		m_PlayingTime = std::fmod(m_PlayingTime + GameApplication::Instance->GetAppContext()->GetGameTime()->GetDeltaTime(), m_CurrAnimationClip->GetClipLength());

		if (m_IsTrasition == true)
		{
			m_PrePlayingTime = std::fmod(m_PrePlayingTime + GameApplication::Instance->GetAppContext()->GetGameTime()->GetDeltaTime(), m_PreAnimationClip->GetClipLength());
		}

		const int _snapCount = m_CurrAnimationClip->GetAnimationSnapList().size();

		for (uint32 i = 1; i < _snapCount; i++)
		{
			auto& _snap = m_CurrAnimationClip->GetAnimationSnapList()[i];

			Vector3 _pos = AnimationClip::GetLerpedVector3(_snap.m_PosKeyFrameList, m_CurrAnimationClip->GetClipLength(), m_PlayingTime);
			Quaternion _rot = AnimationClip::GetSlerpedQuaternion(_snap.m_RotKeyFrameList, m_CurrAnimationClip->GetClipLength(), m_PlayingTime);
			Vector3 _scale = AnimationClip::GetLerpedVector3(_snap.m_ScaleKeyFrameList, m_CurrAnimationClip->GetClipLength(), m_PlayingTime);
			Matrix _tm = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_rot) * Matrix::CreateTranslation(_pos);

			if(m_IsTrasition == true)
			{
				bool _isBlend = IsBlend();

				if (_isBlend == true)
				{
					auto& _preSnap = m_PreAnimationClip->GetAnimationSnapList()[i];

					Vector3 _prepos = AnimationClip::GetLerpedVector3(_preSnap.m_PosKeyFrameList, m_PreAnimationClip->GetClipLength(), m_PrePlayingTime);
					Quaternion _prerot = AnimationClip::GetSlerpedQuaternion(_preSnap.m_RotKeyFrameList, m_PreAnimationClip->GetClipLength(), m_PrePlayingTime);
					Vector3 _prescale = AnimationClip::GetLerpedVector3(_preSnap.m_ScaleKeyFrameList, m_PreAnimationClip->GetClipLength(), m_PrePlayingTime);
					Matrix _pretm = Matrix::CreateScale(_prescale) * Matrix::CreateFromQuaternion(_prerot) * Matrix::CreateTranslation(_prepos);

					_tm = Matrix::Lerp(_pretm, _tm, CalculateBlendRatio());
				}
				else
				{
					Log::Engine_Debug("transition");
					m_IsTrasition = false;
				}
			}

			m_CurrTarget.targets[i]->GetComponent<Transform>()->SetLocalTM(_tm);
		}

		return false;
	}

	void Animator::Tick()
	{
		if (AnimatorTick()) return;
	}

	void Animator::PostTick()
	{
		__super::PostTick();
	}

	void Animator::OnStart()
	{
		__super::OnStart();
	}

	void Animator::OnChangeEnable(bool _newEnable)
	{
		__super::OnChangeEnable(_newEnable);
	}

	ComponentBase* Animator::CreateInstance_Impl(GameObject* _owner)
	{
		return new Animator(_owner);
	}

	ComponentBase* Animator::CloneInstance_Impl(ComponentBase* _cloned, AddressConvertMap* _map)
	{
		ComponentBase::CloneInstance_Impl(_cloned, _map);

		Animator* _anim = dynamic_cast<Animator*>(_cloned);
		_anim->m_PlayingTime = m_PlayingTime;
		_anim->m_PrePlayingTime = m_PrePlayingTime;
		_anim->m_IsPlaying = m_IsPlaying;
		_anim->m_CurrAnimationClip = m_CurrAnimationClip;
		_anim->m_CurrTarget = m_CurrTarget;
		_anim->m_CurrIndex = m_CurrIndex;
		_anim->m_PreAnimationClip = m_PreAnimationClip;
		_anim->m_PreIndex = m_PreIndex;
		_anim->m_AnimationClipList = m_AnimationClipList;

		return _anim;
	}

	void Animator::CloneConvertAddress_Impl(AddressConvertMap* _map)
	{
		ComponentBase::CloneConvertAddress_Impl(_map);

		std::for_each(m_CurrTarget.targets.begin(), m_CurrTarget.targets.end(),
			[_map](GameObject*& _target) { _target = _map->GetMappedTarget<GameObject>(_target); });
	}

	void Animator::Play(const tstring& name)
	{
		if (m_IsPlaying && name == m_CurrAnimationClip->GetClipName())
			return;

		if (m_CurrAnimationClip == nullptr)
			return;

		//m_IsPlaying = false;

		// �ٲ�� �� ���¸� ����
		m_PreAnimationClip = m_CurrAnimationClip;
		m_PreIndex = m_CurrIndex;

		uint _size = m_AnimationClipList.size();
		for (int i = 0; i < _size; ++i)
		{
			auto _clip = m_AnimationClipList[i];

			if (_clip->GetClipName() != name)
				continue;

			m_CurrAnimationClip = _clip;
			m_CurrIndex = i;
			m_IsPlaying = true;

			return;
		}
	}

	void Animator::Stop()
	{
		if (m_IsPlaying == false)
			return;

		m_IsPlaying = false;
		m_PlayingTime = 0.0f;
		m_PrePlayingTime = 0.0f;

		for (uint32 i = 0; i < m_CurrAnimationClip->GetAnimationSnapList().size(); i++)
		{
			if (m_CurrTarget.targets[i] == nullptr)
				return;

			m_CurrTarget.targets[i]->GetComponent<Transform>()->SetLocalTM(m_CurrTarget.initialLocalTMs[i]);
		}
	}

	Resource<AnimationClip>& Animator::GetAnimationClip(uint32 index)
	{
		assert(index < m_AnimationClipList.size());

		return m_AnimationClipList[index];
	}

	Resource<AnimationClip>& Animator::GetAnimationClip(const tstring& clipName)
	{
		const auto _iter = std::find_if(m_AnimationClipList.begin(), m_AnimationClipList.end(),
			[&clipName](Resource<AnimationClip>& _clip) { return _clip->GetClipName() == clipName; });
		assert(_iter != m_AnimationClipList.end());
		return *_iter;
	}

	void Animator::AddClip(Resource<AnimationClip>& clip)
	{
		assert(clip != nullptr);

		auto _iter = m_AnimationClipList.emplace_back(clip);

		InitializeTargetList(clip);
	}

	void Animator::RemoveClip(Resource<AnimationClip>& clip)
	{
		const auto _iter = std::find(m_AnimationClipList.begin(), m_AnimationClipList.end(), clip);
		assert(_iter != m_AnimationClipList.end());
		m_AnimationClipList.erase(_iter);
	}

	void Animator::RemoveClip(const tstring& name)
	{
		const auto _iter = std::find_if(m_AnimationClipList.begin(), m_AnimationClipList.end(),
			[&name](Resource<AnimationClip>& _clip) { return _clip->GetClipName() == name; });
		assert(_iter != m_AnimationClipList.end());
		m_AnimationClipList.erase(_iter);
	}

	void Animator::InitializeTargetList()
	{
		if (m_CurrTarget.bInit)
		{
			m_CurrTarget.targets.clear();
			m_CurrTarget.initialLocalTMs.clear();
		}

		m_CurrTarget.targets.reserve(m_CurrAnimationClip->GetAnimationSnapList().size());
		m_CurrTarget.bInit = true;
		for (uint32 i = 0; i < m_CurrAnimationClip->GetAnimationSnapList().size(); i++)
		{
			auto& _snap = m_CurrAnimationClip->GetAnimationSnapList()[i];

			auto* _findObject = GetGameObject()->FindChildRecursive(_snap.m_TargetName);

			m_CurrTarget.targets.emplace_back(_findObject);
			m_CurrTarget.initialLocalTMs.emplace_back(_findObject->GetComponent<Transform>()->GetLocalTM());
		}
	}

	void Animator::InitializeTargetList(Resource<AnimationClip>& _clip)
	{
		if (m_CurrTarget.bInit)
		{
			m_CurrTarget.targets.clear();
			m_CurrTarget.initialLocalTMs.clear();
		}

		const int _snapCount = _clip->GetAnimationSnapList().size();

		m_CurrTarget.targets.reserve(_snapCount);
		m_CurrTarget.bInit = true;

		for (uint32 i = 0; i < _snapCount; i++)
		{
			const auto& _snap = _clip->GetAnimationSnapList()[i];
			auto* const _findObject = GetGameObject()->FindChildRecursive(_snap.m_TargetName);

			m_CurrTarget.targets.emplace_back(_findObject);
			m_CurrTarget.initialLocalTMs.emplace_back(_findObject->GetComponent<Transform>()->GetLocalTM());
		}
	}

	void Animator::StartTransition(tstring _animName)
	{
		m_IsTrasition = true; // ���带 �ϰ���� ������ On // Off

		Play(_animName); // ���� �� �ִϷ� �÷���.

		m_PrePlayingTime = m_PlayingTime; // ���� �ð��� �޾ƿ´�.

		m_BlendStartTime = m_PrePlayingTime;
	}

	bool Animator::IsBlend()
	{
		// �� �� �ϳ��� nullptr �̶�� �ִϸ��̼��� ���ٴ� ��.  
		assert(m_PreAnimationClip != nullptr);
		assert(m_CurrAnimationClip != nullptr);

		// 1. �ִϸ��̼� ���� ���� �ð��� ���� �ϴ� ���. 
		//const float _blendStartTime = (float)m_PreAnimationClip->GetClipLength() - (_duration);
		//
		//// Time���� ������ �ɸ� ��Ȯ���� �ʾƼ� ���������� ���� ����.
		//const float _playingFrame = m_PlayingTime * 60;
		//const float _blendStartFrame = _blendStartTime * 60;
		//const float _blendEndFrame = m_PreAnimationClip->GetClipLength() * 60;

		// 2. ���� �����ӿ��� ���� �ð� ���� ���� �ϴ� ���.

		const float _blendStartTime = m_BlendStartTime; // �� �κ��� condition�� �ٲ�� ������ �ѹ��� ���;���.
		const float _blendEndTime = m_BlendStartTime + m_Duration;

		// �ִϸ��̼��� �ٲ�����Ƿ� ���� �ִϸ��̼��� m_PlayingTime�� �ƴ� ���� �ִϸ��̼��� time�� ���������.
		const float _playingTime = std::fmod(m_PrePlayingTime + GameApplication::Instance->GetAppContext()->GetGameTime()->GetDeltaTime(), m_PreAnimationClip->GetClipLength());

		const float _playingFrame = _playingTime * 60;
		const float _blendStartFrame = _blendStartTime * 60;
		const float _blendEndFrame = _blendEndTime * 60;

		//Log::Engine_Debug("_blendStartFrame : {0}", _blendStartFrame);
		//Log::Engine_Debug("_playingFrame : {0}", _playingFrame);
		//Log::Engine_Debug("_blendEndFrame : {0}", _blendEndFrame);

		if (_playingFrame >= _blendStartFrame && _playingFrame <= _blendEndFrame)
		{
			return true;
		}

		return false;
	}

	float Animator::CalculateBlendRatio()
	{
		//1. �ִϸ��̼� ���� ���� �ð��� ���� �ϴ� ����.
		//float _blendStartTime = (float)m_PreAnimationClip->GetClipLength() - (m_Duration);
		//float _curBlendTime = m_PlayingTime - _blendStartTime; // ���尡 ���� ���� �ð�.
		//
		//float _blendRatio = _curBlendTime / m_Duration;

		// 2. ���� �����ӿ��� ���� �ð� ���� ���� �ϴ� ����.

		const float _blendStartTime = m_BlendStartTime;
		const float _currBlendTime = m_PrePlayingTime - _blendStartTime;

		const float _blendRatio = _currBlendTime / m_Duration;

		if (_blendRatio == 0)
		{
			m_IsTrasition = false; // ������ 0 �� �Ǹ� ���� �� �ð��� ���� ���̹Ƿ� false �ٲ�. 
		}

		return _currBlendTime / m_Duration;
	}
}
