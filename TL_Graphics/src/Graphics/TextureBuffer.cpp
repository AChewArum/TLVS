#include "pch_dx_11.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize, std::string debugName)
	:ShaderResource(dc, resources, pipeline)
	, dc(dc)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;

	if(data)
		resources->buffers->Create(buffer, desc, &initData);
	else
		resources->buffers->Create(buffer, desc, nullptr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = dataSize / 16;//1¿¤·¹¸àÆ®´ç Å©±â = sizeof(float4) = 16

	resources->srvs->Create(srv, srvDesc, buffer);

	if (debugName.length() > 0)
		srv.resource->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.length(), debugName.c_str());
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::StartPartialUpdate()
{
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
}

void TextureBuffer::PartialUpdate(size_t offset, void* data, size_t dataSize)
{
	// Copy Resource Data..
	memcpy((void*)((char*)mappedResource.pData + offset) , data, dataSize);
}

void TextureBuffer::EndPartialUpdate()
{
	// GPU Access UnLock Buffer Data..
	dc->Unmap(buffer, 0);
}

void TextureBuffer::Set(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	ShaderResource::Set(type, slot);
}

void TextureBuffer::Update(void* data, size_t dataSize)
{
	StartPartialUpdate();

	PartialUpdate(0, data, dataSize);

	EndPartialUpdate();
}

