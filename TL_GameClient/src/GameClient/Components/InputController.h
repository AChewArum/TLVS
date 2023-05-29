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
	//��ɹ޴� ������Ʈ��
	CharacterMovement* m_PlayerMovement = nullptr;
	ObjectControl* m_ObjController = nullptr;
	CameraController* m_CamController = nullptr;
	ShiftController* m_ShiftController = nullptr;
	StateMachine* m_Statemachine = nullptr;


	//ƽ���� �ߵ��ϴ� ��ǲ�� �������� ������ִ� �Լ���
	void InputHandle();
	void MoveInput();
	void CamInput();
	void ObjControlInput();
	void ShiftControlInput();


	//�̵����� Ű�� ���ȴ����� �Ǵ� ����� wsadqe
	static bool IsMoveKeyPressed();
	static bool IsMoveKeyUp();
	//�̵�Ű�� �������� �̵��� �����Ѱ�
	bool m_IsMoveKeyEnabled = true;
	//���콺�� ������������ �Ǵ�
	static bool IsMouseMoved();

	//���� ����� ���� �Լ�s
	//{x,x,x} ���·� ��ȯ x=0,1
	TL_Math::Vector3 PressedDirKey();
	//���� ���·� ��ȯ
	float CalculateDirection();
};
