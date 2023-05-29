#pragma once

#include "Resources.h"

#include "Math\TL_Math.h"

#include "Mesh.h"

#include "ILine.h"

#include "ConstantBuffer.h"

class Pipeline;

class Line : public TL_Graphics::ILine
{
public:
	Line(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, TL_Math::Vector3 startPoint, TL_Math::Vector3 endPoint);
	~Line();

	virtual void Draw() override;

private:
	friend class Pipeline;
	ID3D11DeviceContext* dc;
	Resources* resources;
	Pipeline* pipeline;

	TL_Math::Vector3 startPoint; 
	TL_Math::Vector3 endPoint;

	Mesh* mesh;

	__declspec(align(16)) struct Data
	{
		TL_Math::Matrix identity;
	}data;
	ConstantBuffer* transform;
};