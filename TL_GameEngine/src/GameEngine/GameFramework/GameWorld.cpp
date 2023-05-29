#include "GameEngine_pch.h"
#include "GameEngine/GameFramework/GameWorld.h"

#include "Application/GameTime.h"
#include "GameEngine/GameApplication.h"
#include "GameEngine/GameFramework/Debug.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "GameEngine/GameFramework/Scene.h"
#include "GameEngine/GameFramework/Component/Camera.h"

namespace TL_GameEngine
{
	DEFINE_SINGLETON_CLASS(
		GameWorld,
		[](){},
		[](){}
	)

	void GameWorld::Initialize()
	{
		ProceedOnAwakeAndStart();
		AddGameObjectTag(TEXT("Untagged"));
		AddGameObjectTag(TEXT("MainCamera"));
	}

	void GameWorld::Release()
	{
		if (m_ActiveScene != nullptr)
		{
			DestroyImmediate(m_ActiveScene);
			m_ActiveScene = nullptr;
		}

		assert(m_RootGameObjects.empty());
		assert(m_WaitingForDestroyGameObjects.empty());
		assert(m_WaitingForDestroyComponents.empty());
		assert(m_WaitingForAwakeComponents.empty());
		assert(m_Components.empty());
		assert(m_RenderableComponents.empty());
		assert(m_RenderableComponents.empty());

		m_TagMap.clear();
	}

	void GameWorld::ReserveLoadScene(Scene* _scene)
	{
		m_LoadSceneReserved = _scene;
	}

	void GameWorld::OnStart()
	{
		ProceedOnAwakeAndStart();
	}

	void GameWorld::Tick()
	{
		ProceedLoadScene();

		ProceedDestroy();
		ProceedOnAwakeAndStart();

		// Physics Tick

		size_t _count = m_Components.size();
		for (size_t i = 0; i < _count; ++i)
		{
			if (m_Components[i]->GetGameObject()->GetActiveInHierarchy() && m_Components[i]->GetEnable())
			{
				m_Components[i]->PreTick();
			}
		}

		ProceedDestroy();
		ProceedOnAwakeAndStart();

		_count = m_Components.size();
		for (size_t i = 0; i < _count; ++i)
		{
			if (m_Components[i]->GetGameObject()->GetActiveInHierarchy() && m_Components[i]->GetEnable())
			{
				m_Components[i]->Tick();
			}
		}

		ProceedDestroy();
		ProceedOnAwakeAndStart();

		_count = m_Components.size();
		for (size_t i = 0; i < _count; ++i)
		{
			if (m_Components[i]->GetGameObject()->GetActiveInHierarchy() && m_Components[i]->GetEnable())
			{
				m_Components[i]->PostTick();
			}
		}

		ProceedDestroy();
		ProceedOnAwakeAndStart();
	}

	void GameWorld::FixedTick()
	{
		size_t _count = m_Components.size();
		for (size_t i = 0; i < _count; ++i)
		{
			if (m_Components[i]->GetGameObject()->GetActiveInHierarchy() && m_Components[i]->GetEnable())
			{
				m_Components[i]->FixedTick();
			}
		}
	}

	void GameWorld::Render()
	{
		for (const auto& _renderable : m_RenderableComponents)
			_renderable->Render();


		auto _lines = Debug::GetInstance()->m_VLines;
		for (const auto& _renderablelines : _lines)
		{
			auto _bool = GameApplication::Instance->GetGameTime()->GetGameTime() - _renderablelines.m_Time;
			if (_renderablelines.m_Duration > _bool||_renderablelines.m_Duration==0)
			{
				_renderablelines.m_Line->Draw();
			}
		}

	}

	void GameWorld::ReserveDestroy(GameObject* _gameObject)
	{
		m_WaitingForDestroyGameObjects.push_back(_gameObject);
	}

	void GameWorld::ReserveDestroy(ComponentBase* _component)
	{
		m_WaitingForDestroyComponents.push_back(_component);
	}

	uint32 GameWorld::FindLayerIndex(const tstring& _layerName)
	{
		if (m_Layers.size() == 0)
		{
			m_Layers = {L"Layer1", L"Layer2"};
		}

		uint32 index = 0;
		foreach(layerIter, tstring, m_Layers)
		{
			if (*layerIter == _layerName)
			{
				return index;
			}

			index++;
		}

		return -1;
	}

	tstring& GameWorld::FindLayerName(uint32 _layer)
	{
		assert(_layer > m_Layers.size());

		return m_Layers[_layer];
	}

	int GameWorld::FindGameObjectTag(const tstring& _tagName)
	{
		auto _iter = Instance->m_TagMap.find(_tagName);
		assert(_iter != Instance->m_TagMap.end());
		return _iter->second;
	}

	int GameWorld::AddGameObjectTag(const tstring& _tagName)
	{
		auto _iter = Instance->m_TagMap.find(_tagName);
		assert(_iter == Instance->m_TagMap.end());
		GameObjectTag _newTagId = Instance->m_TagMap.size();
		Instance->m_TagMap.insert({ _tagName, _newTagId });
		return _newTagId;
	}
	
    void GameWorld::ProceedDestroy()
    {
	    // 삭제가 예약된 컴포넌트들을 삭제합니다.
        for (const auto _component : m_WaitingForDestroyComponents)
        {
            _component->OnDestroy();

            // 핸들을 가져온 뒤 아무 처리도 하지 않기 때문에
            // 자동으로 소멸됩니다.
            auto _willDestroy = _component->GetGameObject()->GetUniqueHandleComponent(_component);
        }

        // 삭제가 예약된 게임 오브젝트들을 삭제합니다.
        for (const auto _gameObject : m_WaitingForDestroyGameObjects)
        {
            // 핸들을 가져온 뒤 아무 처리도 하지 않기 때문에
            // 자동으로 소멸됩니다.
            auto _willDestroy = _gameObject->GetUniqueHandleThis();
        }

        m_WaitingForDestroyComponents.clear();
        m_WaitingForDestroyGameObjects.clear();
    }
	
	void GameWorld::DestroyImmediate(Scene* _scene)
	{
		assert(_scene != nullptr);
		assert(_scene == m_ActiveScene);

		std::ranges::for_each(_scene->GetAllRootGameObjects(), [this](auto _go) { ReserveDestroy(_go); });
		ProceedDestroy();

		delete _scene;
	}

	void GameWorld::ProceedOnAwakeAndStart()
	{
		for (int i = 0; i < m_WaitingForAwakeComponents.size(); ++i)
		{
			const auto _component = m_WaitingForAwakeComponents[i];
			_component->OnAwake();
		}

		for (int i = 0; i < m_WaitingForAwakeComponents.size(); ++i)
		{
			const auto _component = m_WaitingForAwakeComponents[i];
			_component->OnStart();
		}

		m_WaitingForAwakeComponents.clear();
	}

	void GameWorld::ProceedLoadScene()
	{
		if (m_LoadSceneReserved != nullptr)
		{
			if (m_ActiveScene != nullptr)
			{
				// 열려있던 기존 씬이 존재한다면,
				// 씬을 전환하기 전에 기존 씬의 모든 오브젝트를 파괴합니다.
				DestroyImmediate(m_ActiveScene);
			}

			m_ActiveScene = m_LoadSceneReserved;
			m_LoadSceneReserved = nullptr;
		}
	}

	void GameWorld::AddComponent(ComponentBase* _component)
	{
		assert(std::ranges::find(m_Components, _component) == m_Components.end());
		assert(std::ranges::find(m_WaitingForAwakeComponents, _component) == m_WaitingForAwakeComponents.end());
		assert(std::ranges::find(m_WaitingForDestroyComponents, _component) == m_WaitingForDestroyComponents.end());

		m_WaitingForAwakeComponents.push_back(_component);
		m_Components.push_back(_component);

		// renderable component를 추가합니다.
		if (_component->CheckFlag(ComponentFlag::eRENDER))
		{
			const auto _renderable = dynamic_cast<IRenderableComponent*>(_component);
			assert(std::ranges::find(m_RenderableComponents, _renderable) == m_RenderableComponents.end());
			m_RenderableComponents.push_back(_renderable);
		}
	}

	void GameWorld::RemoveComponent(ComponentBase* _component)
	{
		const auto _iter = std::ranges::find(m_Components, _component);
		assert(_iter != m_Components.end());
		m_Components.erase(_iter);

		if (const auto it = std::ranges::find(m_WaitingForAwakeComponents, _component);
			it != m_WaitingForAwakeComponents.end())
		{
			// Awake 이벤트를 대기하고 있던 목록에 있던 컴포넌트라면,
			// 그 목록에서 삭제합니다.
			m_WaitingForAwakeComponents.erase(it);
		}

		if (_component->CheckFlag(ComponentFlag::eRENDER))
		{
			// renderable 컴포넌트라면,
			// 그 목록에서 삭제합니다.
			const auto _renderable = dynamic_cast<IRenderableComponent*>(_component);
			const auto it = std::ranges::find(m_RenderableComponents, _renderable);
			assert(it != m_RenderableComponents.end());
			m_RenderableComponents.erase(it);
		}
	}

	void GameWorld::PushUniqueHandleAsRoot(std::unique_ptr<GameObject>&& _handle)
	{
		auto _iter = std::find(m_RootGameObjects.begin(), m_RootGameObjects.end(), _handle);
		assert(_iter == m_RootGameObjects.end());

		m_RootGameObjects.push_back(std::move(_handle));
	}

	std::unique_ptr<GameObject> GameWorld::PopUniqueHandleOf(GameObject* _go)
	{
		auto _iter = std::find_if(
			m_RootGameObjects.begin(), m_RootGameObjects.end(),
			[_go](const std::unique_ptr<GameObject>& _handle) { return _handle.get() == _go; });
		assert(_iter != m_RootGameObjects.end());

		std::unique_ptr<GameObject> _outHandle = std::move(*_iter);
		m_RootGameObjects.erase(_iter);
		return std::move(_outHandle);
	}
}
