#include "Ex_IBL.h"

#include "imgui.h"

void Ex_IBL::Init()
{
	input = new ajwCommon::Input();

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	line = TL_Graphics::RenderSystem::Get()->CreateLine({ 0,0,0 }, { 10,20,10 });

	CreateSphere();

	CreateAndSetEnvs();


	lodBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&lod, sizeof(Lod));
}

void Ex_IBL::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(transformBuffer);
	TL_Graphics::RenderSystem::Get()->Return(lodBuffer);
	TL_Graphics::RenderSystem::Get()->Return(irradianceMap);
	TL_Graphics::RenderSystem::Get()->Return(prefilteredEnvMap);
	TL_Graphics::RenderSystem::Get()->Return(iblBRDF);
	TL_Graphics::RenderSystem::Get()->Return(cubeMap);
	TL_Graphics::RenderSystem::Get()->Return(line);
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_IBL::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse || !io.WantCaptureKeyboard)
		input->Update();//키보드 마우스 업데이트

	{

		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

	}

	BoxMove();

}

void Ex_IBL::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();

	TL_Graphics::RenderSystem::Get()->PreRender();

	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	lodBuffer->Update(&lod.lodLevel, sizeof(Lod));
	lodBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);

	SetLights();
}

void Ex_IBL::Render()
{
	box.Render();

	mesh->Set();

	transformBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 10);
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 50);
	TL_Graphics::RenderSystem::Get()->DrawInstanced(22);




	line->Draw();
}

void Ex_IBL::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_IBL::ImGui()
{
	box.ImGui();

	ImGui::Begin("light");

	ImGui::SliderFloat("intensity", &directionalLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("direction", (float*)&directionalLight.direction, 0, 1.0f);

	ImGui::ColorPicker3("color", (float*)&directionalLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f


	//pointLight
	ImGui::SliderFloat("intensityP", &pointLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("attenuation", (float*)&pointLight.attenuation, 0, 1.0f);
	ImGui::SliderFloat3("position", (float*)&pointLight.position, -10.0f, 10.0f);

	ImGui::ColorPicker3("colorP", (float*)&pointLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f


	//spotLight
	ImGui::SliderFloat("intensitySpot", &spotLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("attenuationSpot", (float*)&spotLight.attenuation, 0, 1.0f);
	ImGui::SliderFloat3("positionSpot", (float*)&spotLight.position, -10.0f, 10.0f);

	ImGui::ColorPicker3("colorSpot", (float*)&spotLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::SliderFloat3("directionSpot", (float*)&spotLight.direction, -1.0f, 1.0f);


	ImGui::SliderFloat("angleSpot", &spotLight.spot, 0, 20.0f);

	ImGui::SliderFloat3("CamPos", (float*)&camT.Pos(), -100, 100);


	ImGui::End();
	ImGui::Begin("test");

	ImGui::SliderFloat("LOD", &lod.lodLevel, 0, 11.0f);

	ImGui::End();
}

void Ex_IBL::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.001f;
		camT.Rot().x += input->MouseDiff().y * 0.001f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.01f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.01f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.01f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.01f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.01f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.01f;
}

void Ex_IBL::BoxMove()
{

	if (input->Press(VK_UP))
		box.transform.Rot().x += 0.0003f;
	if (input->Press(VK_DOWN))
		box.transform.Rot().x -= 0.0003f;
	if (input->Press(VK_LEFT))
		box.transform.Rot().y += 0.0003f;
	if (input->Press(VK_RIGHT))
		box.transform.Rot().y -= 0.0003f;

	box.transform.UpdateWorld();

	box.worldBuffer->Update(&(box.transform.GetWorldMatrix()), sizeof(box.transform.GetWorldMatrix()));
}

void Ex_IBL::SetLights()
{
	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&pointLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&spotLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_IBL::CreateAndSetEnvs()
{
	std::wstring ws = L"Valley";

	irradianceMap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/" + ws +L"DiffuseHDR.dds");
	prefilteredEnvMap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/" + ws + L"SpecularHDR.dds");
	iblBRDF = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/ibl_brdf_lut.png");

	irradianceMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);
	prefilteredEnvMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 13);
	iblBRDF->Set(TL_Graphics::E_SHADER_TYPE::PS, 15);
}

#include "FBXLibrary\FBXModelLoader.h"
#include "FBXLibrary\FBXParserData.h"

void Ex_IBL::CreateSphere()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	_result = _loader.Load(L"_DevelopmentAssets/Model/sphere.fbx");

	_loader.FbxConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	attribute.AddData(_prefab->m_MeshList[0].StaticVertex.data(), _prefab->m_MeshList[0].StaticVertex.size() * sizeof(_prefab->m_MeshList[0].StaticVertex[0]));

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC,"Ball",true);


	transformBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(TL_Math::Matrix) * 22);

	__declspec(align(16)) struct Mat
	{
		float metallic = 0.0f;
		float rougness = 0.0f;
	}mat;

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(Mat) * 22);

	transformBuffer->StartPartialUpdate();

	materialBuffer->StartPartialUpdate();


	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 11; i++)
		{
			t.Pos() = { i * 5.0f, j * 5.0f,0.0f };
			t.UpdateWorld();

			transformBuffer->PartialUpdate(sizeof(TL_Math::Matrix) * (i + j * 11), &t.GetWorldMatrix(), sizeof(TL_Math::Matrix));

			mat.metallic = j;
			mat.rougness = i / 10.0f;

			materialBuffer->PartialUpdate(sizeof(Mat)* (i + j * 11), &mat, sizeof(Mat));
		}
	}

	materialBuffer->EndPartialUpdate();

	transformBuffer->EndPartialUpdate();
}


