#pragma once

#include "Export.h"

#include "MeshType.h"

#include "IMesh.h"
#include "IMaterial.h"
#include "IVertex.h"

#include "IShader.h"

#include "IConstantBuffer.h"
#include "ICamera.h"

#include "ITexture.h"
#include "ITextureBuffer.h"

#include "IRenderTargetTexture.h"

#include "ILight.h"

#include "ControlPanel.h"

#include "ILine.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace TL_Graphics
{
	class AJW_GRAPHICS_DLLEXPORT RenderSystem
	{
	public:
		static void Create();
		static void Delete();
		static RenderSystem* Get();

		virtual HRESULT Init() = 0;

		virtual void Clear() = 0;

		virtual void PreRender() = 0;

		virtual void Draw() = 0;
		virtual void Draw(UINT indexCount, UINT startIndexLocation) = 0;
		virtual void DrawInstanced(UINT numInstance) = 0;

		virtual void PostRender() = 0;

		virtual void Present() = 0;

		virtual IShader* CreateShader(TL_Graphics::E_SHADER_TYPE type, std::wstring fileName) = 0;

		virtual IMesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, std::wstring shaderFileName) = 0;

		virtual IMesh* CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, TL_Graphics::E_MESH_TYPE meshType, std::string debugName = "", bool isInstanced = false) = 0;

		virtual IMaterial* CreateMaterial(const MaterialDesc& desc = MaterialDesc()) = 0;

		virtual IConstantBuffer* CreateConstantBuffer( void* data, size_t dataSize) = 0;

		virtual ICamera* CreateCamera() = 0;

		virtual ITexture* CreateTexture( std::wstring fileName) = 0;

		virtual ITextureBuffer* CreateTextureBuffer( void* data, size_t dataSize) = 0;

		virtual IRenderTargetTexture* CreateRenderTargetTexture(float widthRatio = 1.0f, float heightRatio = 1.0f) = 0;

		virtual void UnSetAllRenderTargets() = 0;
		virtual void UnSetRenderTarget(UINT slot) = 0;

		void Return(void* address);//Create한거 지우는 거

		virtual void UpdateWindowSize(UINT width, UINT height) = 0;

		//set Light must be done before drawing objects
		virtual void BeginSetLight() = 0;

		virtual void SetLight(TL_Graphics::DirectionalLight* light) = 0;
		virtual void SetLight(TL_Graphics::PointLight* light) = 0;
		virtual void SetLight(TL_Graphics::SpotLight* light) = 0;

		virtual void EndSetLight() = 0;

		//바로 다음 드로우콜에만 적용된다.
		virtual void DrawWireOnce() = 0;
		virtual void DrawSolidOnce() = 0;

		virtual void SetSolidModeAsDefualt() = 0;
		virtual void SetWireModeAsDefualt() = 0;

		virtual void SetSwapChainRenderTargetView(UINT slot = 0) = 0;

		virtual ID3D11Device* GetDevice() = 0;
		virtual ID3D11DeviceContext* GetDeviceContext() = 0;

		virtual ControlPanel* GetControlPanel() = 0;//후처리 제어용 변수 모음 구조체

		virtual ILine* CreateLine( TL_Math::Vector3 startPoint, TL_Math::Vector3 endPoint) = 0;
	};

}