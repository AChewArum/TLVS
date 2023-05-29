#include "pch_dx_11.h"

#include "DX11Renderer.h"


DX11Renderer::DX11Renderer()
    :device(nullptr), dc(nullptr), hWnd(0)
    
    , swapChain(nullptr)

{
}

DX11Renderer::~DX11Renderer()
{
    SAFE_DELETE(postProcessor);

    SAFE_DELETE(shadow);

    SAFE_DELETE(lights);

    SAFE_DELETE(pipeline);
    SAFE_DELETE(resources);
    SAFE_RELEASE(dc);
    SAFE_RELEASE(device);
    SAFE_RELEASE(swapChain);
}

HRESULT DX11Renderer::Init()
{
    hWnd = GetActiveWindow();
   

    HRESULT hr = S_OK;

    hr = CreateDeviceAndSwapChain();
    if (hr != S_OK) return hr;

    resources = new Resources(device);
    pipeline = new Pipeline(dc, swapChain, &onResizeNotice, resources);

    lights = new Light(dc, resources, pipeline);

    //gBufferRenderPass = new GBufferRenderPass(dc, resources, pipeline, &onResizeNotice);


    postProcessor = new PostProcessor(dc, resources, pipeline, &onResizeNotice);



    return hr;
}

HRESULT DX11Renderer::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = onResizeNotice.GetWidth();
    swapChainDesc.BufferDesc.Height = onResizeNotice.GetHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;//창모드 변경

    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL FeatureLevel;

    UINT createDeviceFlags = {};

#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        createDeviceFlags,
        &FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &FeatureLevel,
        &dc)))
    {
        return hr;
    }

    return hr;
}

void DX11Renderer::DrawWireOnce()
{
    pipeline->DrawWireOnce();
}

void DX11Renderer::DrawSolidOnce()
{
    pipeline->DrawSolidOnce();
}

void DX11Renderer::SetSolidModeAsDefualt()
{
    pipeline->SetSolidModeAsDefualt();
}

void DX11Renderer::SetWireModeAsDefualt()
{
    pipeline->SetWireModeAsDefualt();
}

void DX11Renderer::SetSwapChainRenderTargetView(UINT slot)
{
    pipeline->SetSwapChainRenderTargetView(slot);
}

ID3D11Device* DX11Renderer::GetDevice()
{
    return device;
}

ID3D11DeviceContext* DX11Renderer::GetDeviceContext()
{
    return dc;
}

void DX11Renderer::BeginSetLight()
{
    lights->BeginLightSet();
}

void DX11Renderer::SetLight(TL_Graphics::DirectionalLight* light)
{
    lights->SetLight(light);
}

void DX11Renderer::SetLight(TL_Graphics::PointLight* light)
{
    lights->SetLight(light);
}

void DX11Renderer::SetLight(TL_Graphics::SpotLight* light)
{
    lights->SetLight(light);
}

void DX11Renderer::EndSetLight()
{
    lights->EndLightSet();
}

Line* DX11Renderer::CreateLine(TL_Math::Vector3 startPoint, TL_Math::Vector3 endPoint)
{
    return new Line(dc, resources, pipeline, startPoint, endPoint);
}

void DX11Renderer::PreRender()
{
    postProcessor->CollectGBufferInfos();

    pipeline->SetDepthEnabled();
}

void DX11Renderer::PostRender()
{
    pipeline->SetStatesDefualt();
    UnSetAllRenderTargets();
    //gBufferRenderPass->SetGBuffers();

    SetSwapChainRenderTargetView();

    postProcessor->Execute();

}



void DX11Renderer::Clear()
{
    float color[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
    pipeline->Clear(color);
    shadow->ClearRTTs();
}

void DX11Renderer::Present()
{
    swapChain->Present(0, 0);
}

Shader* DX11Renderer::CreateShader(TL_Graphics::E_SHADER_TYPE type, std::wstring fileName)
{
    return new Shader(dc, resources, pipeline, type, fileName);
}

Mesh* DX11Renderer::CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, wstring fileName)
{
    return new Mesh(dc, resources, pipeline, vertexSet, indexData, indexCount, fileName);
}

Mesh* DX11Renderer::CreateMesh(TL_Graphics::VertexAttribute& vertexSet, UINT indexData[], UINT indexCount, TL_Graphics::E_MESH_TYPE meshType, std::string debugName, bool isInstanced)
{
    if (!isInstanced)
    {
        if(meshType ==  TL_Graphics::E_MESH_TYPE::STATIC)
            return new Mesh(dc, resources, pipeline,    vertexSet, indexData, indexCount,  L"Shader/TL_StaticMeshVS.hlsl", "Static", debugName);
        else if(meshType ==     TL_Graphics::E_MESH_TYPE::SKINNING)
            return new Mesh(dc, resources, pipeline,    vertexSet, indexData, indexCount,  L"Shader/TL_SkinningMeshVS.hlsl", "Skinning", debugName);
        else if(meshType ==     TL_Graphics::E_MESH_TYPE::SCREENSPACE)
            return new Mesh(dc, resources, pipeline,    vertexSet, indexData, indexCount,  L"Shader/TL_ScreenSpaceMeshVS.hlsl", "Screen", debugName);
    }
    else
    {
        if (meshType == TL_Graphics::E_MESH_TYPE::STATIC)
            return new Mesh(dc, resources, pipeline, vertexSet, indexData, indexCount, L"Shader/TL_StaticMeshVS_Instanced.hlsl");

        assert(false);//미구현
       /* else if (meshType == TL_Graphics::E_MESH_TYPE::SKINNING)
            return new Mesh(resources, pipeline, vertexSet, indexData, indexCount, L"Shader/TL_SkinningMeshVS_Instanced.hlsl");
        else if (meshType == TL_Graphics::E_MESH_TYPE::SCREENSPACE)
            return new Mesh(resources, pipeline, vertexSet, indexData, indexCount, L"Shader/TL_ScreenSpaceMeshVS_Instanced.hlsl");*/

    }

    assert(meshType != TL_Graphics::E_MESH_TYPE::NONE);
}

Material* DX11Renderer::CreateMaterial(const TL_Graphics::MaterialDesc& desc)
{
    return new Material(dc, resources, pipeline, desc);
}

ConstantBuffer* DX11Renderer::CreateConstantBuffer(void* data, size_t dataSize)
{
    return new ConstantBuffer(dc, resources, pipeline, data, dataSize);
}

Camera* DX11Renderer::CreateCamera()
{
    static bool flag = true;

    cam = new Camera(dc, resources, pipeline, &onResizeNotice, 80.0f, 1.0f, 2000.0f);


    if (flag)
    {
        shadow = new Shadow(dc, resources, pipeline, &onResizeNotice, cam, (UINT)1);
        flag = false;
    }

    return cam;
}

Texture* DX11Renderer::CreateTexture(std::wstring fileName)
{

    return new Texture(dc, resources, pipeline, fileName);
}

TextureBuffer* DX11Renderer::CreateTextureBuffer(void* data, size_t dataSize)
{
    return new TextureBuffer(dc, resources, pipeline, data, dataSize);
}

RenderTargetTexture* DX11Renderer::CreateRenderTargetTexture(float widthRatio , float heightRatio )
{
    return new RenderTargetTexture(dc, resources, pipeline, &onResizeNotice, widthRatio, heightRatio);
}

void DX11Renderer::UnSetAllRenderTargets()
{
    pipeline->UnSetAllRenderTargets();
}

void DX11Renderer::UnSetRenderTarget(UINT slot)
{
    pipeline->UnSetRenderTarget(slot);
}

void DX11Renderer::UpdateWindowSize(UINT width, UINT height)
{
    onResizeNotice.Update(width, height);
}

void DX11Renderer::Draw()
{
    pipeline->Draw();

    shadow->Execute();

}

void DX11Renderer::Draw(UINT indexCount, UINT startIndexLocation)
{
    pipeline->Draw(indexCount, startIndexLocation);
    shadow->Execute(indexCount, startIndexLocation);

}

void DX11Renderer::DrawInstanced(UINT numInstance)
{
    pipeline->DrawInstanced(numInstance);
}

TL_Graphics::ControlPanel* DX11Renderer::GetControlPanel()
{
    return &postProcessor->control;
}
