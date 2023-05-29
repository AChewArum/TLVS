#include "Ex_TextureBuffer.h"

void Ex_TextureBuffer::Init()
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

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TextureBufferVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture( L"_DevelopmentAssets/Texture/CJY.jpg");


	transforms.resize(4);
	for (int i = 0; i < 4; i++)
		transforms[i] = new Transform();

	transforms[0]->Pos() = { -2,2,0 };
	transforms[1]->Pos() = { 2,2,0 };
	transforms[2]->Pos() = { -2,-2,0 };
	transforms[3]->Pos() = { 2,-2,0 };

	for (int i = 0; i < 4; i++)
		transforms[i]->UpdateWorld();

	matrixes.resize(4);
	for (int i = 0; i < 4; i++)
		matrixes[i] = transforms[i]->GetWorldMatrix();

	textureBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(matrixes.data(), matrixes.size() * sizeof(TL_Math::Matrix) );
}

void Ex_TextureBuffer::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Get()->Return(texture);
	TL_Graphics::RenderSystem::Get()->Return(textureBuffer);


	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_TextureBuffer::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	{
		input->Update();//키보드 마우스 업데이트

		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

		camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);

		{//파이프라인을 채운다
			material->Set();

			mesh->Set();

			texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

			textureBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);


			TL_Graphics::RenderSystem::Get()->DrawInstanced(4);//파이프 라인의 내용을 이행(렌더타겟에 Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}

void Ex_TextureBuffer::CameraMove()
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
