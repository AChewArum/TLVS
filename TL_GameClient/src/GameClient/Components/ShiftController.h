#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

class ObjectAttribute;

namespace TL_GameEngine
{
	class Transform;
}

class ShiftController :
	public TL_GameEngine::ComponentBase
{
public:
	ShiftController(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("ShiftController"))
		: ComponentBase(_gameObject, _typeName) {}

	void OnStart() override;

	void Tick() override;
	void Convert2D();
	void Convert3D();
	void SetVisibilty(bool _on);
	void SetCollision(bool _on);
	bool SetObject();
	void ShiftScale(TL_Math::Vector3 _impactPoint);

	TL_GameEngine::Transform* MyT() const { return m_MyT; }
	void MyT(TL_GameEngine::Transform* _myT) { m_MyT = _myT; }

	//TODO:
	//±× ¹¹³Ä »çÁø Âï´Â°Å ÀÖÀÝ¾î 
	//void CaptureObject();

private:
	//TRUE = 2D, FALSE = 3D
	ObjectAttribute* m_TargetAtt = nullptr;
	TL_GameEngine::Transform* m_MyT = nullptr;
	TL_GameEngine::Transform* m_ObjT = nullptr;

	TL_Math::Vector3 m_OriginLoc;
	TL_Math::Vector3 m_OriginScl;
	TL_Math::Quaternion m_OriginRot;

	TL_Math::Vector3 m_ImpactLoc;
};
