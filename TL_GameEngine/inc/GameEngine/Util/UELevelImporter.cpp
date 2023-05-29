#include "GameEngine_pch.h"
#include "UELevelImporter.h"

#include <filesystem>

#include "UELevelParser.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/Resource/Resources.h"

namespace TL_GameEngine
{
    void UELevelImporter::Import(Scene* _scene, const tstring& _path)
    {
        const tstring _fileName = std::filesystem::path{_path}.filename();

        std::ifstream _fs(_path);
        std::stringstream buffer;
        buffer << _fs.rdbuf();

        auto _level = UELevelParser::Parse(buffer);

        GameObject* _root = GameObject::Spawn(_scene);
        _root->SetName(_fileName);

        auto _staticMeshActors = _level.GetActorsHasComponent("StaticMeshComponent");
        for (auto a : _staticMeshActors)
        {
            auto _components = a.GetComponentsOf("StaticMeshComponent");
            auto _c = _components[0];

            auto _staticMeshComponent = UELevelParser::Component_StaticMeshComponent::Parse(_c);
            if (_staticMeshComponent.meshFileNameWithoutExtension.empty())
                continue;

            const tstring _fileName = StringHelper::ToTString(_staticMeshComponent.meshFileNameWithoutExtension) + TEXT(".fbx");
            const tstring _fileAbsolutePath = Resources::GetResourcePathByName(_fileName);

            if (_fileAbsolutePath.empty())
                continue;

            Vector3& _worldPosition = _staticMeshComponent.relativeLocation;
            Vector3 _worldRotation = _staticMeshComponent.relativeRotation;
            Vector3& _worldScale = _staticMeshComponent.relativeScale3D;

            auto _go = Resources::GetInstance()->LoadFBXPrefab(_scene, _fileAbsolutePath);
            _go->SetName(StringHelper::ToTString(a.name));

            _go->SetParent(_root);
            _go->m_Transform.SetLocalScale(_worldScale);
            
            _go->m_Transform.SetLocalRotation(Quaternion::CreateFromAxisAngleDeg(Vector3::Up, -90.f) * Quaternion::CreateFromYawPitchRollDeg(_worldRotation));

            _go->m_Transform.SetLocalPosition(_worldPosition);
        }

        _root->m_Transform.SetWorldScale(Vector3::One * 0.1f);
    }
}
