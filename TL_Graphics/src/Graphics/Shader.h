#pragma once

#include <d3d11.h>

#include "Resources.h"

#include "ShaderType.h"

#include "IShader.h"

class Pipeline;

class Shader : public TL_Graphics::IShader
{
public:
	Shader(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Graphics::E_SHADER_TYPE type, std::wstring fileName, std::string debugName = "");
	virtual ~Shader();

	void Set();
	void* SetTest();
	void SetOnce();

	operator ID3D11VertexShader* () const { return shader.vs.resource; }
	operator ID3D11VertexShader* const* () const { return &(shader.vs.resource); }
	operator ID3D11PixelShader* () const { return shader.ps.resource; }
	operator ID3D11PixelShader* const* () const { return &(shader.ps.resource); }


public:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	TL_Graphics::E_SHADER_TYPE type;

	union _shader//유니언 뿌 ㅡ 듯 코드 좀 귀여운듯.. ㅎ
	{
		_shader() {};
		~_shader() {};


		Resource<ID3D11VertexShader> vs{};
		Resource<ID3D11PixelShader> ps;

	}shader;
};