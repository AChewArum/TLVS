#include "GameEngine_pch.h"
#include "GameEngine/Resource/GameObjectPrefab.h"

namespace TL_GameEngine
{
    tstring GameObjectPrefab::Serialize()
    {
        // TODO: 프리팹이 참조하는 게임 오브젝트를 시리얼라이즈해 반환합니다.

        return TEXT("");
    }

    GameObjectPrefab GameObjectPrefab::Deserialize(const tstring& _data)
    {
        // TODO: 데이터로부터 게임 오브젝트를 디시리얼라이즈해 생성하고,
        // 그 오브젝트를 가리키는 프리팹을 생성해 반환합니다.

        return GameObjectPrefab{nullptr};
    }
}
