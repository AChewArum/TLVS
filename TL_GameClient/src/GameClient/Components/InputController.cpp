#include "InputController.h"
#include "GameEngine\Gameframework\Physics\PhysicsSystem.h"
#include "GameEngine/Gameframework/Physics/RaycastHit.h"
#include "ObjectControl.h"
#include "CameraController.h"
#include "PlayerAttribute.h"
#include "CharacterMovement.h"
#include "ShiftController.h"
#include "GameEngine/GameFramework/GameObject.h"
#include "GameEngine/GameFramework/Component/Collider.h"

using namespace TL_GameEngine;
using namespace TL_Application;

void InputController::OnStart()
{
	ComponentBase::OnStart();
	m_PlayerMovement = m_GameObject->GetComponent<CharacterMovement>();
	m_ObjController = m_GameObject->GetComponent<ObjectControl>();
	m_CamController = m_GameObject->GetComponent<CameraController>();
	m_ShiftController = m_GameObject->GetComponent<ShiftController>();
}

void InputController::Tick()
{
	ComponentBase::Tick();
	CamInput();
}

void InputController::FixedTick()
{
	ComponentBase::FixedTick();
	InputHandle();
}

void InputController::InputHandle()
{
	if (Input::GetKeyDown(EVirtualKey::MouseLeft)) { }
	else if (Input::GetKeyDown(EVirtualKey::MouseRight)) { }

	MoveInput();
	ObjControlInput();
	ShiftControlInput();
}

bool InputController::IsMoveKeyPressed()
{
	const bool _b = (Input::GetKeyPress(EVirtualKey::Key_W) ||
			Input::GetKeyPress(EVirtualKey::Key_S)) ||
		(Input::GetKeyPress(EVirtualKey::Key_A) ||
			Input::GetKeyPress(EVirtualKey::Key_D)) ||
		(Input::GetKeyPress(EVirtualKey::Key_Q) ||
			Input::GetKeyPress(EVirtualKey::Key_E));
	return _b;
}

bool InputController::IsMoveKeyUp()
{
	const bool _b = (Input::GetKeyUp(EVirtualKey::Key_W) ||
			Input::GetKeyUp(EVirtualKey::Key_S)) ||
		(Input::GetKeyUp(EVirtualKey::Key_A) ||
			Input::GetKeyUp(EVirtualKey::Key_D)) ||
		(Input::GetKeyUp(EVirtualKey::Key_Q) ||
			Input::GetKeyUp(EVirtualKey::Key_E));
	return _b;
}

float InputController::CalculateDirection()
{
	Vector3 _v = PressedDirKey();
	float _forward = _v.x;
	float _right = _v.y;
	float _ang = atan2f(_right, _forward);


	return _ang;
}

TL_Math::Vector3 InputController::PressedDirKey()
{
	float _forward = 0;
	float _right = 0;
	float _clock = 0;
	if (Input::GetKeyPress(EVirtualKey::Key_W))
	{
		_forward += 1;
	}
	if (Input::GetKeyPress(EVirtualKey::Key_S))
	{
		_forward -= 1;
	}
	if (Input::GetKeyPress(EVirtualKey::Key_A))
	{
		_right -= 1;
	}
	if (Input::GetKeyPress(EVirtualKey::Key_D))
	{
		_right += 1;
	}
	if (Input::GetKeyPress(EVirtualKey::Key_Q))
	{
		_clock -= 1;
	}
	if (Input::GetKeyPress(EVirtualKey::Key_E))
	{
		_clock += 1;
	}

	return {_forward, _right, _clock};
}

bool InputController::IsMouseMoved()
{
	Vector2 _v = Input::GetMouseMovementDelta();
	if (_v.Length() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void InputController::MoveInput()
{
	if (m_PlayerMovement == nullptr) return;
	if (m_IsMoveKeyEnabled == false) return;
	if (IsMoveKeyPressed())
	{
		m_PlayerMovement->Move(CalculateDirection());
	}

	else if (IsMoveKeyUp)
	{
		m_PlayerMovement->Stop();
	}

	if (Input::GetKeyDown(EVirtualKey::SpaceBar))
	{
		m_PlayerMovement->Jump();
	}

	//if (Input::GetKeyDown(EVirtualKey::LeftShift))
	//{
	//	m_PlayerMovement->IsRunning(true);
	//}
	//if (Input::GetKeyUp(EVirtualKey::LeftShift))
	//{
	//	m_PlayerMovement->IsRunning(false);
	//}
}

void InputController::CamInput()
{
	if (m_CamController == nullptr) return;

	if (IsMouseMoved())
	{
		m_CamController->Rotate();
		m_PlayerMovement->Rotate();
	}
}

void InputController::ObjControlInput()
{
	if (m_ObjController == nullptr) return;
	if (Input::GetKeyDown(EVirtualKey::Key_G))
	{
		m_ObjController->SetGrab();
	}
	if (Input::GetKeyDown(EVirtualKey::Key_R))
	{
		m_IsMoveKeyEnabled = false;
	}
	if (Input::GetKeyUp(EVirtualKey::Key_R))
	{
		m_IsMoveKeyEnabled = true;
	}
	if (IsMoveKeyPressed() && m_IsMoveKeyEnabled == false)
	{
		m_ObjController->RotateObject(PressedDirKey());
	}
}

void InputController::ShiftControlInput()
{
	if (m_ShiftController == nullptr) return;
	if (Input::GetKeyDown(EVirtualKey::Key_3))
	{
		m_ShiftController->Convert2D();
	}
	else if (Input::GetKeyDown(EVirtualKey::Key_4))
	{
		m_ShiftController->Convert3D();
	}
}
