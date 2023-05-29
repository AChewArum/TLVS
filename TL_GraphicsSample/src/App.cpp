#include "App.h"

#include "Example\00_Ex_Triagle\Ex_Triangle.h"
#include "Example\01_Ex_TriangleColor\Ex_TriangleColor.h"
#include "Example\02_Ex_BoxCamera\Ex_BoxCamera.h"
#include "Example\03_Ex_Texture\Ex_Texture.h"
#include "Example\04_Ex_TextureBuffer\Ex_TextureBuffer.h"
#include "Example\05_Ex_StateChange\Ex_StateChange.h"
#include "Example\06_EX_RTT\EX_RTT.h"
#include "Example\07_EX_PBR\EX_PBR.h"
#include "Example\08_Ex_Light\Ex_Light.h"
#include "Example\09_Ex_Hyobin\Ex_Hyobin.h"
#include "Example\10_Ex_Deferred\Ex_Deferred.h"
#include "Example\11_Ex_PostProsessing\Ex_PostProsessing.h"

#include "Example\12_Ex_IBL\Ex_IBL.h"

App::App()
{
	//example = new Ex_Triangle();
	//example = new Ex_TriangleColor();
	//example = new Ex_BoxCamera();
	//example = new Ex_Texture();
	//example = new Ex_TextureBuffer();
	//example = new Ex_StateChange();
	//example = new EX_RTT();
	//example = new EX_PBR();
	//example = new Ex_Light();
	example = new Ex_Hyobin();
	//example = new Ex_Deferred();
	//example = new Ex_PostProsessing();
	//example = new Ex_IBL();

	//example = new Ex_Light();


	example->Init();
}

App::~App()
{
	example->UnInit();
}

void App::Update()
{
	example->Update();
}

void App::PreRender()
{
	example->PreRender();
}

void App::Render()
{
	example->Render();
}

void App::PostRender()
{
	example->PostRender();
}

void App::ImGui()
{
	example->ImGui();
}
