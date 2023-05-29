#pragma once

#include "ITextureBuffer.h"

#include "ShaderResource.h"


class TextureBuffer : public TL_Graphics::ITextureBuffer, public ShaderResource
{
public:
	TextureBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize, std::string debugName = "");
	virtual ~TextureBuffer();

	virtual void StartPartialUpdate() override;

	virtual void PartialUpdate(size_t offset, void* data, size_t dataSize) override;

	virtual void EndPartialUpdate() override;

	virtual void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot) override;

	virtual void Update(void* data, size_t dataSize) override;


private:
	Resource<ID3D11Buffer> buffer;
	ID3D11DeviceContext* dc;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
};