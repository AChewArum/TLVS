#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "IConstantBuffer.h"

class Pipeline;

class ConstantBuffer : public TL_Graphics::IConstantBuffer
{
public:
	ConstantBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, void* data, size_t dataSize);
	virtual ~ConstantBuffer();

	virtual void Set(TL_Graphics::E_SHADER_TYPE type,		UINT slot) override;
	ID3D11Buffer* SetTest(TL_Graphics::E_SHADER_TYPE type, UINT slot);
	void SetOnce(TL_Graphics::E_SHADER_TYPE type,		UINT slot);

	virtual void Update(void* data, size_t dataSize) override;

	void SetDebugName(std::string debugName);
	

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11Buffer> buffer;
};