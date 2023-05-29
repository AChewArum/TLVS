#include "Ex_StateChange.h"

void Ex_StateChange::Init()
{
	input = new ajwCommon::Input();


	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	a.UpdateColor({ 1,0,0,1 });
	b.UpdateColor({ 0,1,0,1 });
	b.UpdatePos({ 10,0,0 });
	c.UpdateColor({ 0,0,1,1 });
	c.UpdatePos({ -10,0,0 });


}

void Ex_StateChange::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_StateChange::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	{
		input->Update();//키보드 마우스 업데이트
	}

	{
		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

		camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	}

	if (input->Down(VK_F1))
		TL_Graphics::RenderSystem::Get()->SetSolidModeAsDefualt();
	if (input->Down(VK_F2))
		TL_Graphics::RenderSystem::Get()->SetWireModeAsDefualt();

	TL_Graphics::RenderSystem::Get()->DrawWireOnce();
		a.Render();
	//TL_Graphics::RenderSystem::Get()->DrawSolidOnce();

		b.Render();
		c.Render();

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}

void Ex_StateChange::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.01f;
		camT.Rot().x += input->MouseDiff().y * 0.01f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.01f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.01f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.01f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.01f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.01f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.01f;
}
