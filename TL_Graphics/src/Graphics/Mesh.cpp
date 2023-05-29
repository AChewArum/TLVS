#include "pch_dx_11.h"
#include "Mesh.h"

#include "Pipeline.h"

Mesh::Mesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, wstring vsFileName, std::string debugNameShader, std::string debugName, D3D_PRIMITIVE_TOPOLOGY topology)
	:vertexDataSize(vertexSet.pimpl->GetVertexSize()) 
	,dc(dc)
	,indexCount(indexCount)
	, vsFileName(vsFileName)
	, topology(topology)
	, resources(resources)
	,pipeline(pipeline)
{
	desc = vertexSet.pimpl->GetDescsData();
	descSize = vertexSet.pimpl->GetDescsSize();

	//inputLayout
	resources->inputLayouts->Get(inputLayout, desc, descSize, vsFileName);

	shader = new Shader(dc, resources, pipeline, TL_Graphics::E_SHADER_TYPE::VS, vsFileName, debugNameShader);

	//shader
	//resources->vertexShaders->Get(shader, vsFileName);

	//vertexBuffer;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = vertexSet.pimpl->GetVerticesSize();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertexSet.pimpl->GetVerticesData();

	resources->buffers->Create(vertexBuffer, vbd, &vinitData);

	//index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indexData;

	resources->buffers->Create(indexBuffer, ibd, &iinitData);

	if (debugName.length() > 0)
	{
		string temp = debugName + "Vertex";
		vertexBuffer.resource->SetPrivateData(WKPDID_D3DDebugObjectName, temp.length(), temp.c_str());
		temp = debugName + "Index";
		indexBuffer.resource->SetPrivateData(WKPDID_D3DDebugObjectName, temp.length(), temp.c_str());
	}
}

Mesh::~Mesh()
{
	SAFE_DELETE(shader);
}

void Mesh::Set()
{
	shader->Set();
	pipeline->SetMesh(this);
}
