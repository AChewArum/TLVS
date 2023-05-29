#include "Ex_Texture.h"

void Ex_Texture::Init()
{
	input = new ajwCommon::Input();

	struct Vertex
	{
		float position[3];
		float uv[2];
	}
	vertices[4]
	{
		//앞면
		{{-0.5f,        +0.5f,      -0.5f}, { 0,0}},
		{{+0.5f,        +0.5f,      -0.5f}, { 1,0} },
		{{-0.5f,        -0.5f,      -0.5f}, { 0,1}},
		{{+0.5f,        -0.5f,      -0.5f}, { 1,1} }
	};

	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,1,2,
		1,3,2
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TextureVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer( &(squareT.GetWorldMatrix()), sizeof(squareT.GetWorldMatrix()));

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CJY.jpg");
}

void Ex_Texture::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
	TL_Graphics::RenderSystem::Get()->Return(texture);


	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_Texture::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	{
		input->Update();//키보드 마우스 업데이트

		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		//camera->Update(camT.GetWorldMatrix());

		camera->Update(camT.Pos(), camT.Quat());

		camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);


		{//파이프라인을 채운다
			material->Set();

			mesh->Set();

			TransformMove();

			squareT.UpdateWorld();

			worldBuffer->Update(&(squareT.GetWorldMatrix()), sizeof(squareT.GetWorldMatrix()));

			worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

			texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

			TL_Graphics::RenderSystem::Get()->Draw();//파이프 라인의 내용을 이행(렌더타겟에 Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}

void Ex_Texture::CameraMove()
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

void Ex_Texture::TransformMove()
{
	if (input->Press(VK_UP))
		squareT.Pos().y += 0.01f;
	if (input->Press(VK_DOWN))
		squareT.Pos().y -= 0.01f;
	if (input->Press(VK_RIGHT))
		squareT.Pos().x += 0.01f;
	if (input->Press(VK_LEFT))
		squareT.Pos().x -= 0.01f;
}
