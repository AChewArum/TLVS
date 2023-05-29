#include "Box.h"

#include "Graphics\IVertex.h"

Box::Box()
{
	struct Vertex
	{
		float position[3];
		float normal[3];
	}
	vertices[24]
	{
		//¾Õ¸é
		{{-0.5f,        +0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        +0.5f,      -0.5f},{0,0,-1}},
		{{-0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},

		//µÞ¸é
		{{-0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{-0.5f,         -0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         -0.5f,     +0.5f},{0,0,1}},

		//¿Þ¸é
		{{-0.5f,        +0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        +0.5f,      -0.5f},{-1,0,0}},
		{{-0.5f,        -0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        -0.5f,      -0.5f}	,{-1,0,0}},

		//¿À¸¥¸é
		{{+0.5f,         +0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         +0.5f,     +0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     +0.5f},	{1,0,0}},

		//À­¸é
		{{-0.5f,        +0.5f,      +0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      +0.5f}	,{0,1,0}},
		{{-0.5f,        +0.5f,      -0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      -0.5f}		,{0,1,0}},

		//¾Æ·§¸é
		{{-0.5f,         -0.5f,     +0.5f}	,{0,-1,0}},
		{{+0.5f,         -0.5f,     +0.5f}	,{0,-1,0}},
		{{-0.5f,         -0.5f,     -0.5f}		,{0,-1,0}},
		{{+0.5f,         -0.5f,     -0.5f}	,{0,-1,0}}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");

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
		8,9,10,
		9,11,10,
		//right
		12,13,14,
		13,15,14,

		//up
		16,17,18,
		17,19,18,
		//down
		20,	22,21,
		21,	22,23
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/BoxVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));

	colorBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&color, sizeof(TL_Math::Vector4));
}

Box::~Box()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
	TL_Graphics::RenderSystem::Get()->Return(colorBuffer);
}

void Box::Render()
{
	mesh->Set();
	material->Set();
	worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);
	colorBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->Draw();
}

void Box::UpdateColor(TL_Math::Vector4 color)
{
	this->color = color;
	colorBuffer->Update(&this->color, sizeof(TL_Math::Vector4));
}

void Box::UpdatePos(TL_Math::Vector3 pos)
{
	transform.Pos() = pos;
	transform.UpdateWorld();

	worldBuffer->Update(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));
}

