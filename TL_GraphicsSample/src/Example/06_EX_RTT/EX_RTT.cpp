#include "EX_RTT.h"

void EX_RTT::Init()
{
	input = new ajwCommon::Input();


	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	for (UINT i = 0; i < 4; i++)
		rtt[i] = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();

	a.UpdateColor({ 1,0,0,1 });
	b.UpdateColor({ 0,1,0,1 });
	b.UpdatePos({ 10,0,0 });
	c.UpdateColor({ 0,0,1,1 });
	c.UpdatePos({ -10,0,0 });

	/////////////////// canvas

	struct Vertex
	{
		float position[3];
		float uv[2];
	}
	vertices[4]
	{
		//앞면
		{{-1.0f,        1.0f,      0.0f}, {0.0f, 0.0f}},//LT
		{{1.0f,        1.0f,      0.0f}, {1.0f, 0.0f}},//RT
		{{-1.0f,       -1.0f,      0.0f}, {0.0f, 1.0f}},//LB
		{{1.0f,        -1.0f,      0.0f}, {1.0f, 1.0f}} //RB
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		0,1,2,
		1,3,2,
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/CanvasVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CJY.jpg");

}

void EX_RTT::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(texture);

	TL_Graphics::RenderSystem::Get()->Return(material);

	TL_Graphics::RenderSystem::Get()->Return(mesh);

	for (UINT i = 0; i < 4; i++)
		TL_Graphics::RenderSystem::Get()->Return(rtt[i]);

	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void EX_RTT::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고
	for (UINT i = 0; i < 4; i++)
		rtt[i]->Clear({ 0.0f, 0.7f, 1.0f, 1.0f });

	{
		input->Update();//키보드 마우스 업데이트
	}

	{
		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

		camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	}

	for (UINT i = 0; i < 4; i++)
		rtt[i]->SetRT(i);//1번슬롯에 꽂는다.

	a.Render();
	b.Render();
	c.Render();

	for (UINT i = 0; i < 4; i++)
		TL_Graphics::RenderSystem::Get()->UnSetRenderTarget(i);

	TL_Graphics::RenderSystem::Get()->SetSwapChainRenderTargetView();

	{//Canvas
		mesh->Set();

		material->Set();	

		for (UINT i = 0; i < 4; i++)
		rtt[i]->SetT(TL_Graphics::E_SHADER_TYPE::PS, i);

		//texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

		TL_Graphics::RenderSystem::Get()->Draw();

	}

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}

void EX_RTT::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.01f;
		camT.Rot().x += input->MouseDiff().y * 0.01f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.005f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.005f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.005f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.005f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.005f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.005f;
}
