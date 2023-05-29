#include "pch_dx_11.h"
#include "Pipeline.h"

Pipeline::Pipeline(ID3D11DeviceContext* dc, IDXGISwapChain* swapChain, OnResizeNotice* resizeNotice, Resources* resources)
	:dc(dc), currentMaterial(nullptr), currentMesh(nullptr)
	, swapChain(swapChain)
	, resources(resources)
{
	resizeNotice->AddObserver(this);

	swapChainRtv = new SwapChainRenderTarget(dc, resources, this, resizeNotice);

	Init(resizeNotice->GetWidth(), resizeNotice->GetHeight());

	texInfoBuffer = new ConstantBuffer(dc, resources, this, texInfos, sizeof(TexInfo) * 4096);
}

Pipeline::~Pipeline()
{
	SAFE_DELETE(texInfoBuffer);

	SAFE_DELETE(swapChainRtv);
}

void Pipeline::Init(UINT width, UINT height)
{
	CreateDefaultStates();

	SetStatesDefualt();

	OnResize(width, height);

	swapChainRtv->OnResize(width, height);

	SetDepthStencilView(depthStencilView);
}

void Pipeline::OnResize(uint32_t width, uint32_t height)
{
	ResizeDepthStencilView(width, height);
	ResizeViewPort(width, height);
}

void Pipeline::SetMesh(Mesh* mesh)//check
{
	currentInputLayout = mesh->inputLayout;
	currentPrimitiveTopology = mesh->topology;

	currentVertexBufferInfo.ptrBuffer = mesh->vertexBuffer;
	currentVertexBufferInfo.strides = &mesh->vertexDataSize;
	currentVertexBufferInfo.offset = &mesh->offset;

	currentIndexBuffer = mesh->indexBuffer;

	currentMesh = mesh;
}

void Pipeline::Clear(float color[4])
{
	dc->ClearRenderTargetView(swapChainRtv->rtv, color);

	dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Pipeline::ClearRenderTarget(ID3D11RenderTargetView* renderTarget, TL_Math::Vector4 color)
{
	float rgba[4] = { color.x,color.y,color.z,color.w };
	dc->ClearRenderTargetView(renderTarget, rgba);
}

void Pipeline::SetStatesDefualt()
{
	SetCurrentRasterState(defaultRasterState);
	SetCurrentDepthStencilState(depthEnabledDepthStencilState);
	SetCurrentBlendState(defaultBlendState);
	SetCurrentSamplerState(wrapSamplerState, 0);
	SetCurrentSamplerState(mirrorSamplerState, 1);
	SetCurrentSamplerState(clampSamplerState, 2);
	SetCurrentSamplerState(borderSamplerState, 3);

}

ID3D11RasterizerState* Pipeline::SetCurrentRasterState(ID3D11RasterizerState* state)
{
	ID3D11RasterizerState* old = currentRasterState;
	currentRasterState = state;
	return old;
}

void Pipeline::SetCurrentRasterStateOnce(ID3D11RasterizerState* state)
{
	ID3D11RasterizerState* old = state;
	SetCurrentRasterState(state);

	reservations.emplace_back(
		[=]()
		{
			SetCurrentRasterState(old);
		}
	);
}

D3D11_VIEWPORT* Pipeline::SetViewPort(D3D11_VIEWPORT* viewport)
{
	D3D11_VIEWPORT* old = currentViewport;
	currentViewport = viewport;
	return old;
}

void Pipeline::SetViewPortOnce(D3D11_VIEWPORT* viewport)
{
	D3D11_VIEWPORT* old = currentViewport;
	SetViewPort(viewport);

	reservations.emplace_back(
		[=]()
		{
			SetViewPort(old);
		}
	);
}

ID3D11Buffer* Pipeline::SetConstantBuffer(ID3D11Buffer* constantBuffer, TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	ID3D11Buffer* old = nullptr;
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		old = currentConstantBuffersVS[slot];
		currentConstantBuffersVS[slot] = constantBuffer;
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		old = currentConstantBuffersPS[slot];
		currentConstantBuffersPS[slot] = constantBuffer;
	}

	return old;
}

void Pipeline::SetConstantBufferOnce(ID3D11Buffer* constantBuffer, TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		ID3D11Buffer* oldBuffer = currentConstantBuffersVS[slot];

		currentConstantBuffersVS[slot] = constantBuffer;

		reservations.emplace_back(
			[=]()
			{
				SetConstantBuffer(oldBuffer, TL_Graphics::E_SHADER_TYPE::VS, slot);
			}
		);
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{

		ID3D11Buffer* oldBuffer = currentConstantBuffersPS[slot];

		currentConstantBuffersPS[slot] = constantBuffer;

		reservations.emplace_back(
			[=]()
			{
				SetConstantBuffer(oldBuffer, TL_Graphics::E_SHADER_TYPE::PS, slot);
			}
		);
	}
}

void Pipeline::SetShaderResource(ShaderResource* shaderResource, TL_Graphics::E_SHADER_TYPE type,
	UINT slot)
{
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		if (shaderResource)
			currentShaderResourceVS[slot] = shaderResource->srv;
		else
			currentShaderResourceVS[slot] = nullptr;
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		if (shaderResource)
			currentShaderResourcePS[slot] = shaderResource->srv;
		else
			currentShaderResourcePS[slot] = nullptr;

	}
}

void Pipeline::UnSetShaderResource(TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	if (type == TL_Graphics::E_SHADER_TYPE::VS)
	{
		currentShaderResourceVS[slot] = nullptr;
	}

	else if (type == TL_Graphics::E_SHADER_TYPE::PS)
	{
		currentShaderResourcePS[slot] = nullptr;
	}
}

void Pipeline::SetTexture(Texture* texture, TL_Graphics::E_SHADER_TYPE type, UINT slot)
{
	TexInfo info = {};

	if (texture)
	{
		info.texX = texture->GetSizeX();
		info.texY = texture->GetSizeY();
		info.texXInv = 1 / (float)info.texX;
		info.texYInv = 1 / (float)info.texY;
	}
	

	texInfos[slot] = info;

	SetShaderResource(texture, type, slot);

	//todo : 여기 조금 비효율 적일지도? 나중에 검토할것
	texInfoBuffer->Update(texInfos, sizeof(TexInfo) * 4096);

	texInfoBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 3);
}

void Pipeline::SetMaterial(Material* material, UINT albdeoMapSlot, UINT metallicMapSlot, UINT roughnessMapSlot)
{
	currentMaterial = material;
}

ID3D11VertexShader* Pipeline::SetShader(ID3D11VertexShader* shader)
{
	ID3D11VertexShader* old = currentShaderVS;
	currentShaderVS = shader;
	return old;
}

ID3D11PixelShader* Pipeline::SetShader(ID3D11PixelShader* shader)
{
	ID3D11PixelShader* old = currentShaderPS;
	currentShaderPS = shader;
	return old;
}

void Pipeline::SetShaderOnce(ID3D11VertexShader* shader)
{
	ID3D11VertexShader* old = currentShaderVS;

	currentShaderVS = shader;

	reservations.emplace_back(
		[=]()
		{
			SetShader(old);
		}
	);
}

void Pipeline::SetShaderOnce(ID3D11PixelShader* shader)
{
	ID3D11PixelShader* old = currentShaderPS;

	currentShaderPS = shader;

	reservations.emplace_back(
		[=]()
		{
			SetShader(old);
		}
	);
}


ID3D11RenderTargetView* Pipeline::SetRenderTarget(ID3D11RenderTargetView* rtv, UINT slot)
{
	ID3D11RenderTargetView* old = currentRenderTarget[slot];
	currentRenderTarget[slot] = rtv;
	return old;
}

void Pipeline::SetRenderTargetOnce(ID3D11RenderTargetView* rtv, UINT slot)
{
	ID3D11RenderTargetView* old[MAX_RENDERTARGET] = {
		currentRenderTarget[0],
		currentRenderTarget[1],
		currentRenderTarget[2],
		currentRenderTarget[3],
		currentRenderTarget[4],
		currentRenderTarget[5],
		currentRenderTarget[6],
		currentRenderTarget[7],
	};

	UnSetAllRenderTargets();

	SetRenderTarget(rtv, slot);

	reservations.emplace_back(
		[=]()
		{
			for (int i = 0; i < MAX_RENDERTARGET; i++)
				SetRenderTarget(old[i], slot);
		}
	);

}

ID3D11DepthStencilView* Pipeline::SetDepthStencilView(ID3D11DepthStencilView* depthStencilView)
{
	ID3D11DepthStencilView* old = currentDepthStencilView;
	currentDepthStencilView = depthStencilView;
	return old;
}

void Pipeline::SetDepthStencilViewOnce(ID3D11DepthStencilView* depthStencilView)
{
	ID3D11DepthStencilView* old = currentDepthStencilView;

	currentDepthStencilView = depthStencilView;

	reservations.emplace_back(
		[=]()
		{
			SetDepthStencilView(old);
		}
	);
}

void Pipeline::SetSwapChainRenderTargetView(UINT slot)
{
	swapChainRtv->Set(slot);
}

void Pipeline::UnSetAllRenderTargets()
{
	for (UINT i = 0; i < MAX_RENDERTARGET; i++)
		UnSetRenderTarget(i);
}

void Pipeline::UnSetRenderTarget(UINT slot)
{
	currentRenderTarget[slot] = nullptr;
}

void Pipeline::SetCurrentBlendState(Resource<ID3D11BlendState> state)
{
	currentBlendState = state;
}

void Pipeline::SetCurrentSamplerState(ID3D11SamplerState* state, UINT slot)
{
	currentSamplerStates[slot] = state;
}


void Pipeline::SetCurrentDepthStencilState(Resource<ID3D11DepthStencilState> state)
{
	currentDepthStencilState = state;
}

void Pipeline::SetPipeline()
{
	//OM
	BindRenderTargets();
	BindBlendStates();
	BindDepthStencilStates();

	//IA
	BindInputLayouts();
	BindPrimitiveTopology();
	BindVertexBuffers();
	BindIndexBuffer();

	//VS
	BindConstantBuffersVS();
	BindShaderResourcesVS();
	BindShaderVS();

	//RS
	BindRasterizerStates();
	BindViewports();

	//PS
	BindConstantBuffersPS();
	BindShaderResourcesPS();
	BindShaderPS();
	BindSamplers();
}

void Pipeline::Draw()
{
	SetPipeline();

	dc->DrawIndexed(currentMesh->GetIndexCount(), 0, 0);

	for (auto reserve : reservations)
		reserve();
	reservations.clear();

	SetTexture(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 3);
}

void Pipeline::Draw(UINT indexCount, UINT startIndexLocation)
{

	SetPipeline();

	dc->DrawIndexed(indexCount, startIndexLocation, 0);



	for (auto reserve : reservations)
		reserve();
	reservations.clear();

	SetTexture(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 3);
}

void Pipeline::DrawInstanced(UINT numInstance)
{
	SetPipeline();

	dc->DrawIndexedInstanced(currentMesh->GetIndexCount(), numInstance, 0, 0, 0);

	for (auto reserve : reservations)
		reserve();
	reservations.clear();

	SetTexture(nullptr, TL_Graphics::E_SHADER_TYPE::PS, 3);
}

void Pipeline::CreateDefaultStates()
{
	resources->rasterStates->GetDefault(solidRasterState);
	resources->rasterStates->GetWireMode(wireFrameRasterState);

	defaultRasterState = solidRasterState;
	//defaultRasterState = wireFrameRasterState;

	resources->depthStencilStates->GetDepthEnabled(depthEnabledDepthStencilState);
	resources->depthStencilStates->GetDepthDisabled(depthDisabledDepthStencilState);
	resources->blendStates->GetDefault(defaultBlendState);

	resources->samplerStates->GetWrap(wrapSamplerState);
	resources->samplerStates->GetMirror(mirrorSamplerState);
	resources->samplerStates->GetClamp(clampSamplerState);
	resources->samplerStates->GetBorder(borderSamplerState);
}

void Pipeline::ResizeDepthStencilView(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	resources->texture2Ds->Create(depthStencilBuffer, depthBufferDesc);

	resources->depthStencilViews->CreateDefault(depthStencilView, depthStencilBuffer);

	SetDepthStencilView(depthStencilView);

	assert(SUCCEEDED(hr));
}

void Pipeline::ResizeViewPort(UINT width, UINT height)
{
	viewPort.Width = (float)width;
	viewPort.Height = (float)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	SetViewPort(&viewPort);
}

void Pipeline::DrawWireOnce()
{
	SetCurrentRasterState(wireFrameRasterState);

	reservations.emplace_back(
		[&]()
		{
			SetCurrentRasterState(defaultRasterState);
		}
	);
}

void Pipeline::DrawSolidOnce()
{

	SetCurrentRasterState(solidRasterState);

	reservations.emplace_back(
		[&]()
		{
			SetCurrentRasterState(defaultRasterState);
		}
	);
}

void Pipeline::SetSolidModeAsDefualt()
{
	defaultRasterState = solidRasterState;
	SetCurrentRasterState(defaultRasterState);
}

void Pipeline::SetWireModeAsDefualt()
{
	defaultRasterState = wireFrameRasterState;
	SetCurrentRasterState(defaultRasterState);
}

void Pipeline::SetDepthEnabled()
{
	SetCurrentDepthStencilState(depthEnabledDepthStencilState);
}

void Pipeline::SetDepthDisabled()
{
	SetCurrentDepthStencilState(depthDisabledDepthStencilState);
}

void Pipeline::BindRenderTargets()
{
	dc->OMSetRenderTargets(MAX_RENDERTARGET, currentRenderTarget, currentDepthStencilView);
}

void Pipeline::BindBlendStates()
{
	dc->OMSetBlendState(currentBlendState, NULL, 0xFF);
}

void Pipeline::BindDepthStencilStates()
{
	dc->OMSetDepthStencilState(currentDepthStencilState, 1);
}

void Pipeline::BindInputLayouts()
{
	dc->IASetInputLayout(currentInputLayout);
}

void Pipeline::BindPrimitiveTopology()
{
	dc->IASetPrimitiveTopology(currentPrimitiveTopology);
}

void Pipeline::BindVertexBuffers()
{
	dc->IASetVertexBuffers(0, 1, currentVertexBufferInfo.ptrBuffer, currentVertexBufferInfo.strides, currentVertexBufferInfo.offset);
}

void Pipeline::BindIndexBuffer()
{
	dc->IASetIndexBuffer(currentIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::BindConstantBuffersVS()
{
	dc->VSSetConstantBuffers(0, 14, currentConstantBuffersVS);
}

void Pipeline::BindShaderResourcesVS()
{
	dc->VSSetShaderResources(0, 128, currentShaderResourceVS);
}

void Pipeline::BindShaderVS()
{
	dc->VSSetShader(currentShaderVS, 0, 0);
}

void Pipeline::BindRasterizerStates()
{
	dc->RSSetState(currentRasterState);
}

void Pipeline::BindViewports()
{
	dc->RSSetViewports(1, currentViewport);
}

void Pipeline::BindConstantBuffersPS()
{
	dc->PSSetConstantBuffers(0, 14, currentConstantBuffersPS);
}

void Pipeline::BindShaderResourcesPS()
{
	dc->PSSetShaderResources(0, 128, currentShaderResourcePS);
}

void Pipeline::BindShaderPS()
{
	dc->PSSetShader(currentShaderPS, 0, 0);
}

void Pipeline::BindSamplers()
{
	dc->PSSetSamplers(0, 16, currentSamplerStates);
}
