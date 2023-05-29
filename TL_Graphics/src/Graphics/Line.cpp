#include "pch_dx_11.h"
#include "Line.h"

#include "IVertex.h"

#include "Pipeline.h"

Line::Line(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Math::Vector3 startPoint, TL_Math::Vector3 endPoint)
	:dc(dc)
	, resources(resources)
	, pipeline(pipeline)
{

	struct Vertex
	{
		float position[3];

		float uv[2];

		float normal[3];
		float tangent[3];
		float bitangent[3];
	}vertices[2]
	{
		{{startPoint.x, startPoint.y, startPoint.z}, { 0.0f, 0.0f }, { 0,0,1 }, { -1,0,0 }, { 0,1,0 }},
		{{endPoint.x, endPoint.y, endPoint.z}, { 0.0f, 0.0f }, { 0,0,1 }, { -1,0,0 }, { 0,1,0 }}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indices[]
		= { 0,1 };

	

	//todo : ½¦ÀÌ´õ µû·Î ¸¸µé±â ±ÍÂú¾Æ¼­ ±âÁ¸ ½¦ÀÌ´õ ÀÏ´Ü ¾¸, ³ªÁß¿¡ ²À °íÄ¥ °Í
	mesh = new Mesh(dc, resources, pipeline, vertexAttribute, indices, 2, L"Shader/TL_StaticMeshVS.hlsl","Static","Line", D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	transform = new ConstantBuffer(dc, resources, pipeline, &data, sizeof(Data));
}

Line::~Line()
{
	SAFE_DELETE(transform);
	SAFE_DELETE(mesh);
}

void Line::Draw()
{
	mesh->Set();

	transform->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

	pipeline->Draw();
}
