#include "Ex_TriangleColor.h"

void Ex_TriangleColor::Init()
{

    struct Vertex
    {
        float position[2];
    }
    vertices[3]
    {
        {0.0f, 0.5f}
        ,{-0.5f, -0.5f}
        ,{0.5f, -0.5f}
    };
    vertexAttribute.AddElementToDesc(sizeof(float) * 2, TL_Graphics::DataType::FLOAT, "POSITION");
    vertexAttribute.AddData(vertices, sizeof(vertices));
    UINT indicies[]
        =
    {
        0,2,1
    };

    currentMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, 3, L"TriangleColorVS.hlsl");

    currentMaterial = TL_Graphics::RenderSystem::Get()->CreateMaterial();

    struct Color
    {
        float rgba[4];
    }color{1,0,1,1};

    constantBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer( &color, sizeof(Color));
}

void Ex_TriangleColor::UnInit()
{
    TL_Graphics::RenderSystem::Delete();
}

void Ex_TriangleColor::Update()
{
    TL_Graphics::RenderSystem::Get()->Clear();

    currentMaterial->Set();

    currentMesh->Set();

    constantBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);

    TL_Graphics::RenderSystem::Get()->Draw();

    TL_Graphics::RenderSystem::Get()->Present();
}
