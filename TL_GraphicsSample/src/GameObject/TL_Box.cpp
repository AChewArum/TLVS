#include "TL_Box.h"

#include "Graphics\IVertex.h"

TL_Box::TL_Box()
{
	struct Vertex
	{
		float position[3];

		float uv[2];

		float normal[3];
		float tangent[3];
		float bitangent[3];
	}
	vertices[24]//´ëÃæ ¸¸µë ¾ö¹ÐÇÑ µ¥ÀÌÅÍ x
	{
		//¾Õ¸é
		{{-0.5f,+0.5f,+0.5f},{0.0f, 0.0f},{0,0,1},{-1,0,0},{0,1,0}},
		{{+0.5f,+0.5f,+0.5f},{1.0f, 0.0f},{0,0,1},{-1,0,0},{0,1,0}},
		{{-0.5f,-0.5f,+0.5f},{0.0f, 1.0f},{0,0,1},{-1,0,0},{0,1,0}},
		{{+0.5f,-0.5f,+0.5f},{1.0f, 1.0f},{0,0,1},{-1,0,0},{0,1,0}},

		//µÞ¸é
		{{-0.5f,+0.5f,-0.5f},{0.0f, 0.0f},{0,0,-1},{1,0,0},{0,-1,0}},
		{{+0.5f,+0.5f,-0.5f},{1.0f, 0.0f},{0,0,-1},{1,0,0},{0,-1,0}},
		{{-0.5f,-0.5f,-0.5f},{0.0f, 1.0f},{0,0,-1},{1,0,0},{0,-1,0}},
		{{+0.5f,-0.5f,-0.5f},{1.0f, 1.0f},{0,0,-1},{1,0,0},{0,-1,0}},

		//¿Þ¸é
		{{+0.5f,+0.5f,-0.5f},{0.0f, 0.0f},{1,0,0},{-1,0,0},{0,1,0}},
		{{+0.5f,+0.5f,+0.5f},{1.0f, 0.0f},{1,0,0},{-1,0,0},{0,1,0}},
		{{+0.5f,-0.5f,-0.5f},{0.0f, 1.0f},{1,0,0},{-1,0,0},{0,1,0}},
		{{+0.5f,-0.5f,+0.5f},{1.0f, 1.0f},{1,0,0},{-1,0,0},{0,1,0}},

		//¿À¸¥¸é
		{{-0.5f,+0.5f,+0.5f},{0.0f, 0.0f},{-1,0,0},{1,0,0},{0,-1,0}},
		{{-0.5f,+0.5f,-0.5f},{1.0f, 0.0f},{-1,0,0},{1,0,0},{0,-1,0}},
		{{-0.5f,-0.5f,+0.5f},{0.0f, 1.0f},{-1,0,0},{1,0,0},{0,-1,0}},
		{{-0.5f,-0.5f,-0.5f},{1.0f, 1.0f},{-1,0,0},{1,0,0},{0,-1,0}},

		//À­¸é
		{{-0.5f,-0.5f,+0.5f},{0.0f, 0.0f},{0,-1,0},{-1,0,0},{0,1,0}},
		{{+0.5f,-0.5f,+0.5f},{1.0f, 0.0f},{0,-1,0},{-1,0,0},{0,1,0}},
		{{-0.5f,-0.5f,-0.5f},{0.0f, 1.0f},{0,-1,0},{-1,0,0},{0,1,0}},
		{{+0.5f,-0.5f,-0.5f},{1.0f, 1.0f},{0,-1,0},{-1,0,0},{0,1,0}},

		//¾Æ·§¸é
		{{-0.5f,+0.5f,+0.5f},{0.0f, 0.0f},{0,1,0},{-1,0,0},{0,1,0}},
		{{+0.5f,+0.5f,+0.5f},{1.0f, 0.0f},{0,1,0},{-1,0,0},{0,1,0}},
		{{-0.5f,+0.5f,-0.5f},{0.0f, 1.0f},{0,1,0},{-1,0,0},{0,1,0}},
		{{+0.5f,+0.5f,-0.5f},{1.0f, 1.0f},{0,1,0},{-1,0,0},{0,1,0}}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,1,2,
		1,3,2,
		//back
		5,4,6,
		5,6,7,

		//left
		8,10,9,
		9,10,11,
		//right
		12,14,13,
		13,14,15,

		//up
		16,17,18,
		17,19,18,
		//down
		20,	22,21,
		21,	22,23
	};

	//mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TL_StaticMeshVS.hlsl");
	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), TL_Graphics::E_MESH_TYPE::STATIC);
	//mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TL_SkyBoxVS.hlsl");

	TL_Graphics::MaterialDesc matDesc;
	matDesc.baseColor_opcityFilePath = L"_DevelopmentAssets/Texture/CJY.jpg";

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));

	pixelShader = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TestObjectGBuffers.hlsl");

	matInfo = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(Material));

}

TL_Box::~TL_Box()
{
	TL_Graphics::RenderSystem::Get()->Return(matInfo);
	TL_Graphics::RenderSystem::Get()->Return(pixelShader);
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
}

void TL_Box::Render()
{
	pixelShader->Set();
	mesh->Set();
	material->Set();

	matInfo->Update(&mat, sizeof(Material));
	matInfo->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);
	worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

	TL_Graphics::RenderSystem::Get()->Draw();
}

#include "imgui.h"

void TL_Box::ImGui()
{
	ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("metallic", &mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();
}
