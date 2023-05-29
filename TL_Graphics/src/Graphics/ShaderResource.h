#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "ShaderType.h"

class Pipeline;


class ShaderResource
{
public:
	ShaderResource(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::string debugName = "");
	virtual ~ShaderResource();

	void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot);

protected:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	Resource<ID3D11ShaderResourceView> srv;
};