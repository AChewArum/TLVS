#pragma once

#include "Mesh.h"

class Pipeline;

class ScreenMesh
{
public:
	ScreenMesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
	~ScreenMesh();

	void Set();

private:
	Mesh* mesh;

	void CreateMesh(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline);
};