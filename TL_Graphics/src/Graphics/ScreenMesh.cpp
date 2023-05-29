#include "pch_dx_11.h"
#include "ScreenMesh.h"

#include "IVertex.h"

ScreenMesh::ScreenMesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
{
	CreateMesh(dc, resources, pipeline);
}

ScreenMesh::~ScreenMesh()
{
	delete mesh;
}

void ScreenMesh::Set()
{
	mesh->Set();
}

void ScreenMesh::CreateMesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline)
{
	struct Vertex
	{
		float position[2];
		float uv[2];
	}
	vertices[4]
	{
		//¾Õ¸é
		{{-1.0f,        1.0f}, {0.0f, 0.0f}},//LT
		{{1.0f,        1.0f}, {1.0f, 0.0f}},//RT
		{{-1.0f,       -1.0f}, {0.0f, 1.0f}},//LB
		{{1.0f,        -1.0f}, {1.0f, 1.0f}} //RB
	};

	TL_Graphics::VertexAttribute attribute;
	attribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	attribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		0,2,1,
		1,2,3,
	};

	mesh = new Mesh(dc, resources, pipeline, attribute, indicies, 6, L"Shader/TL_ScreenSpaceMeshVS.hlsl", "Screen", "Screen");
}
