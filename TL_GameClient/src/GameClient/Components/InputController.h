#pragma once
#include "CameraController.h"

#include "GameEngine/GameFramework/Component/ComponentBase.h"
#include "Application/Input.h"

class StateMachine;
class ShiftController;
class ObjectControl;
class CharacterMovement;
class CameraController;

class InputController :
	public TL_GameEngine::ComponentBase
{
public:
	InputController(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("InputHandler"))
		: ComponentBase(_gameObject, _typeName) {}

	void OnStart() override;
	void Tick() override;
	void FixedTick() override;
private:
	//명령받는 컴포넌트들
	CharacterMovement* m_PlayerMovement = nullptr;
	ObjectControl* m_ObjController = nullptr;
	CameraController* m_CamController = nullptr;
	ShiftController* m_ShiftController = nullptr;
	StateMachine* m_Statemachine = nullptr;


	//틱에서 발동하는 인풋이 들어왔을때 명령해주는 함수들
	void InputHandle();
	void MoveInput();
	void CamInput();
	void ObjControlInput();
	void ShiftControlInput();


	//이동관련 키가 눌렸는지를 판단 현재는 wsadqe
	static bool IsMoveKeyPressed();
	static bool IsMoveKeyUp();
	//이동키를 눌렀을대 이동이 가능한가
	bool m_IsMoveKeyEnabled = true;
	//마우스가 움직였는지를 판단
	static bool IsMouseMoved();

	//방향 계산을 위한 함수s
	//{x,x,x} 형태로 반환 x=0,1
	TL_Math::Vector3 PressedDirKey();
	//각도 형태로 반환
	float CalculateDirection();
};
