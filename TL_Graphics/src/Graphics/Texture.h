#pragma once

#include "ITexture.h"

#include "ShaderResource.h"

class Texture : public TL_Graphics::ITexture, public ShaderResource
{
public:
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::wstring fileName, std::string debugName = "");
	Texture(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, std::string debugName = "");
	virtual ~Texture();
	
	virtual void Set(TL_Graphics::E_SHADER_TYPE type,
		UINT slot) override;

	UINT GetSizeX() { return sizeX; }
	UINT GetSizeY() { return sizeY; }

protected:
	std::wstring fileName;


	void LoadTexInfo();
	///if sizeZ = 0; 2d, sizeZ >= 1 3d
	UINT sizeX = 0, sizeY = 0, sizeZ = 0;
};