//
// Created by letsd on 13-Aug-24.
//

#define GUI_ENABLE


#include "DX12Renderer.h"

#include <assert.h>
#include <dxgi1_6.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <DXGIDebug.h>
#endif

#ifdef GUI_ENABLE
#include "../../../Gui/DX12/DX12Imgui.h"
#endif


#include <array>
#include <vector>
#include <wrl.h>
#include <sstream>
#include <string>

#include "RenderTexture.h"
#include "../../../Logger/Logger.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "Utility/CpuDescriptorHeap.h"
#include "Utility/D3DUtillity.h"
#include "Utility/d3dx12.h"
#include "Utility/DescriptorHeap.h"
#include "Utility/GpuDescriptorHeap.h"
#include "Utility/GPUDescriptorHeapManager.h"
#include "Utility/UploadBuffer.h"
#include "Utility/MeshGeometry.h"



namespace BINDU {

    using namespace Microsoft::WRL;
    using namespace DirectX;

    struct ObjectConstants
    {
        XMFLOAT4X4    WorldViewProjMat;
    };

    struct Vertex
    {
        XMFLOAT3   Position;
        XMFLOAT4   Color;
    };

    class DX12Renderer::Impl
    {
    public:
        ~Impl() {}

        // Pointer to the DX12Renderer class
        static DX12Renderer* m_dx12Renderer;

        // Methods
        void InitDirect3D();

        void InitDXGI();

        UINT CheckMSAASupport(ID3D12Device* pd3dDevice, UINT sampleCount, DXGI_FORMAT backBufferFormat);

        void CreateCommandObjects(ID3D12Device* pd3dDevice, ComPtr<ID3D12CommandQueue>& pCmdQueue, ComPtr<ID3D12CommandAllocator>& pCmdAllocator, ComPtr<ID3D12GraphicsCommandList>& pCmdList);

        void CreateSwapChain(IDXGIFactory4* pDXGIFactory, IDXGISwapChain1** pSwapChain, ID3D12CommandQueue* pCommandQueue, DXGI_FORMAT backBufferFormat, UINT bufferCount);

        std::vector<ComPtr<IDXGIAdapter>> EnumAdapters(IDXGIFactory* pDXGIfactory);
        void EnumAdapterOutputs(IDXGIAdapter* pAdapter);
        void EnumOutputDisplayModes(IDXGIOutput* pOutput, DXGI_FORMAT backBufferFormat);

        void CreateRootSignature();
        void CreateInputLayout();
        void CreateVertexAndIndexBuffers();
        void CompileShaders();
        void CreatePSO();

        void CreateRtvHeap();
        void CreateDsvHeap();
        void CreateCbvHeapAndView();

        void CreateRtv();
        void CreateDsv(UINT height, UINT width);

#ifdef GUI_ENABLE
        void CreateSrvHeap();
        void CreateRenderTexture();
#endif


        void CreateViewportAndScissorRect();

        // Wait for GPU to finish all the commands prior to this call
        void FlushCommandQueue();

        // Discard the SwapChain and other client area dependent objects and recreate all with the new client area dimension
        void OnResize();

        // Getters
        bool GetMSAAState() const;
        UINT GetMSAASampleCount() const;
        UINT GetMSAAQuality() const;

        // returns descriptor cpu handle to the current Back buffer
        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferCPUDescriptor() const;
        // returns current Render target back buffer resource
        ID3D12Resource* GetCurrentBackBuffer() const;

        // returns descriptor handle to the Depth Stencil buffer
        D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilBufferDescriptor() const;
        // return current DepthStencil buffer resource
        ID3D12Resource* GetDepthStencilBuffer() const;

        // Variables


        // DXGI Stuff

        // DXGI Available adapters
        std::vector<ComPtr<IDXGIAdapter>> m_dxgiAdapters;

        // DXGI Factory
        ComPtr<IDXGIFactory4>    m_dxgiFactory{ nullptr };

        // DXGI SwapChain
        ComPtr<IDXGISwapChain3>  m_dxgiSwapChain{ nullptr };

        // SwapChain Buffers
        UINT m_swapChainBufferCount = SWAP_CHAIN_BUFFER_COUNT;

        ComPtr<ID3D12Resource> m_swapChainBuffers[SWAP_CHAIN_BUFFER_COUNT];

        UINT m_currentBackBuffer = 0;


        // Direct3D Stuff
        std::unique_ptr<RenderTexture> m_renderTexture { nullptr };

        // Direct3D Device
        ComPtr<ID3D12Device>    m_d3dDevice{ nullptr };

        // Direct3D Debug interface
        ComPtr<ID3D12Debug6>    m_d3dDebug{ nullptr };

        // D3D Fence
        UINT64                  m_currentFenceValue = 0;
        ComPtr<ID3D12Fence>     m_fence{ nullptr };

        // D3D Command Objects

        // D3D Command Queue
        ComPtr<ID3D12CommandQueue> m_d3dCommandQueue{ nullptr };
        // D3D Command Allocator
        ComPtr<ID3D12CommandAllocator> m_d3dDirectCmdAllocator{ nullptr };
        // D3D Graphics Command List
        ComPtr<ID3D12GraphicsCommandList> m_d3dCommandList{ nullptr };

        // Root signature
        ComPtr<ID3D12RootSignature>            m_rootSignature{ nullptr };

        // Descriptor heaps

    	// RTV heap
        std::unique_ptr<DescriptorHeap>        m_rtvHeap{ nullptr };
        // SRV heap
        std::unique_ptr<DescriptorHeap>        m_srvHeap{ nullptr };
        // DSV heap
        std::unique_ptr<DescriptorHeap>        m_dsvHeap{ nullptr };
        // CBV heap
        std::unique_ptr<DescriptorHeap>        m_cbvHeap{ nullptr };

        // CBV, SRV, UAV GPU descriptor heap
        std::unique_ptr<DescriptorHeap>        m_cbvSrvUavGpuHeap{ nullptr };
        std::unique_ptr<GPUDescriptorHeapManager> m_gpuDescriptorHeapManager{ nullptr };

        // Input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC>  m_inputElements;

        // Geometry
        std::unique_ptr<MeshGeometry>          m_geometry;

        // Shader bytecodes
        ComPtr<ID3DBlob>                       m_vertexShader;
        ComPtr<ID3DBlob>                       m_pixelShader;

        // Pipeline State
        ComPtr<ID3D12PipelineState>            m_pso{ nullptr };

        // World stuff
        XMFLOAT4X4                             m_worldMat;
        XMFLOAT4X4                             m_viewMat;
        XMFLOAT4X4                             m_projMat;

        float m_theta = 1.5f * XM_PI;
        float m_phi = XM_PIDIV4;
        float m_radius = 5.0f;

        // Descriptor sizes
        UINT m_RtvDHandleIncrementSize = 0;
        UINT m_DsvDHandleIncrementSize = 0;
        UINT m_CbvSrvUavDHandleIncrementSize = 0;
        UINT m_SamplerDHandleIncrementSize = 0;

        // MSAA
        bool m_isMSAAEnabled{ false };
        UINT m_MSAASampleCount = 4;
        UINT m_MSAAQuality = 0;

        // DepthStencil Buffer
        ComPtr<ID3D12Resource> m_depthStencilBuffer;

        // Constant buffer
        std::unique_ptr<UploadBuffer<ObjectConstants>>  m_cbPerObject{ nullptr };

        // Back Buffer and DepthStencil Buffer
        DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        // ViewPort and Scissor rect
        D3D12_VIEWPORT m_viewport;
        D3D12_RECT     m_scissorRect;


        // handle to the output window
        HWND m_hwnd{ nullptr };
        // Window
        IWindow* m_window{ nullptr };


#ifdef GUI_ENABLE
        std::unique_ptr<DX12Imgui>             m_gui{ nullptr };
#endif
    };


    void DX12Renderer::Impl::CreateVertexAndIndexBuffers()
    {

        std::array<Vertex,8>  vertices =
        {
            Vertex({XMFLOAT3(-1.f,-1.f,-1.f), XMFLOAT4(1.f,0.f,0.f,1.f) }),
            Vertex({XMFLOAT3(-1.f,+1.f,-1.f), XMFLOAT4(0.f,1.f,0.f,1.f) }),
            Vertex({XMFLOAT3(+1.f,+1.f,-1.f), XMFLOAT4(0.f,0.f,1.f,1.f) }),
            Vertex({XMFLOAT3(+1.f,-1.f,-1.f), XMFLOAT4(1.f,1.f,0.f,1.f) }),
            Vertex({XMFLOAT3(-1.f,-1.f,+1.f), XMFLOAT4(1.f,1.f,0.f,1.f) }),
            Vertex({XMFLOAT3(-1.f,+1.f,+1.f), XMFLOAT4(0.f,1.f,1.f,1.f) }),
            Vertex({XMFLOAT3(+1.f,+1.f,+1.f), XMFLOAT4(1.f,0.f,1.f,1.f) }),
            Vertex({XMFLOAT3(+1.f,-1.f,+1.f), XMFLOAT4(1.f,1.f,0.f,1.f) })
        };

        std::array<std::uint16_t,36>   indices =
        {
            // front face
            0,1,2,
            0,2,3,

            // back face
            4,6,5,
            4,7,6,

            // left face
            4,5,1,
            4,1,0,

            // right face
            3,2,6,
            3,6,7,

            // top face
            1,5,6,
            1,6,2,

            // bottom face
            4,0,3,
            4,3,7
        };

        UINT vbByteSize = vertices.size() * sizeof(Vertex);

        UINT ibByteSize = indices.size() * sizeof(std::uint16_t);
                

        m_geometry = std::make_unique<MeshGeometry>();
        // Geometry to hold the vertices and indices
        m_geometry->Name = "Geometry";
        m_geometry->VertexBufferByteSize = vbByteSize;
        m_geometry->VertexByteStride = sizeof(Vertex);
        
        m_geometry->IndexBufferByteSize = ibByteSize;
        m_geometry->IndexBufferFormat = DXGI_FORMAT_R16_UINT;
        

        // Create the vertex buffer CPU
        DXThrowIfFailed(D3DCreateBlob(vbByteSize, m_geometry->VertexBufferCPU.GetAddressOf()));
        CopyMemory(m_geometry->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

        // Create the index buffer CPU
        DXThrowIfFailed(D3DCreateBlob(ibByteSize, m_geometry->IndexBufferCPU.GetAddressOf()));
        CopyMemory(m_geometry->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

        // Create the GPU size of vertex and index buffer
        m_geometry->VertexBufferGPU = D3DUtility::CreateDefaultBuffer(m_d3dDevice.Get(),
            m_d3dCommandList.Get(), vertices.data(), vbByteSize, m_geometry->VertexBufferUploader);

        m_geometry->IndexBufferGPU = D3DUtility::CreateDefaultBuffer(m_d3dDevice.Get(),
            m_d3dCommandList.Get(), indices.data(), ibByteSize, m_geometry->IndexBufferUploader);

        SubMeshGeometry boxSubMesh;
        boxSubMesh.IndexCount = indices.size();
        boxSubMesh.BaseVertexLocation = 0;
        boxSubMesh.StartIndexLocation = 0;

        m_geometry->SubMeshMap["Box"] = boxSubMesh;
    }

    void DX12Renderer::Impl::CompileShaders()
    {
        m_vertexShader = D3DUtility::CompileShader("C:\\Users\\letsd\\CLionProjects\\Bindu\\Shaders\\Default\\Default.hlsl", nullptr,
            "VS", "vs_5_0");
        m_pixelShader = D3DUtility::CompileShader("C:\\Users\\letsd\\CLionProjects\\Bindu\\Shaders\\Default\\Default.hlsl", nullptr,
            "PS", "ps_5_0");
    }

    void DX12Renderer::Impl::CreatePSO()
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC  psoDesc;
        ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

        psoDesc.InputLayout.pInputElementDescs = m_inputElements.data();
        psoDesc.InputLayout.NumElements = m_inputElements.size();
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.VS =
        {
            reinterpret_cast<BYTE*>(m_vertexShader->GetBufferPointer()),
            m_vertexShader->GetBufferSize()
        };
        psoDesc.PS =
        {
        reinterpret_cast<BYTE*>(m_pixelShader->GetBufferPointer()),
        m_pixelShader->GetBufferSize()
        };
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.DSVFormat = m_depthStencilFormat;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = m_backBufferFormat;
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.SampleDesc.Count = GetMSAAState() ? GetMSAASampleCount() : 1;
        psoDesc.SampleDesc.Quality = GetMSAAState() ? GetMSAAQuality() : 0;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


        assert(m_d3dDevice != nullptr);

        DXThrowIfFailed(m_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso)));
    }

    void DX12Renderer::Impl::CreateRtvHeap()
    {

        if (!m_rtvHeap)
            m_rtvHeap = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE, RTDescriptors::RTCount);
    }

    void DX12Renderer::Impl::CreateDsvHeap()
    {
        if (!m_dsvHeap)
            m_dsvHeap = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1);
    }

    void DX12Renderer::Impl::CreateCbvHeapAndView()
    {
        m_cbPerObject = std::make_unique<UploadBuffer<ObjectConstants>>(m_d3dDevice.Get(), 1, true);

        if (!m_cbvHeap)
            m_cbvHeap = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = m_cbPerObject->Resource()->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = m_cbPerObject->GetElementByteSize();

        m_d3dDevice->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetFirstCPUHandle());


    }

    void DX12Renderer::Impl::CreateRtv()
    {
        Logger::Get()->Log(LogType::Info, "Creating RTV buffer and resource");


        for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
        {
            // Get the ith buffer from the SwapChain
            m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(m_swapChainBuffers[i].ReleaseAndGetAddressOf()));

            // Create a render target to that buffer
            m_d3dDevice->CreateRenderTargetView(m_swapChainBuffers[i].Get(), nullptr, m_rtvHeap->GetCPUHandleAt(i));
        }

    }

    void DX12Renderer::Impl::CreateDsv(UINT height, UINT width)
    {
        Logger::Get()->Log(LogType::Info, "Creating depth stencil buffer and resource");

        // Depth stencil buffer is a Texture2D. so, create a resource description for it
        D3D12_RESOURCE_DESC rsd = {};
        rsd.Format = DXGI_FORMAT_R24G8_TYPELESS;
        rsd.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        rsd.Height = height;
        rsd.Width = width;
        rsd.Alignment = 0;
        rsd.DepthOrArraySize = 1;
        rsd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rsd.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        rsd.MipLevels = 1;
        rsd.SampleDesc.Count = GetMSAAState() ? GetMSAASampleCount() : 1;
        rsd.SampleDesc.Quality = GetMSAAState() ? (GetMSAAQuality() - 1) : 0;

        // Optimized clear value
        D3D12_CLEAR_VALUE optClear = {};
        optClear.Format = m_depthStencilFormat;
        optClear.DepthStencil.Depth = 1.0f;
        optClear.DepthStencil.Stencil = 0.f;

        // Property of the heap this depth stencil buffer resource will live in
        CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);

        DXThrowIfFailed(m_d3dDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &rsd, D3D12_RESOURCE_STATE_COMMON, &optClear,
            IID_PPV_ARGS(m_depthStencilBuffer.ReleaseAndGetAddressOf())));

        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
        dsvDesc.Format = m_depthStencilFormat;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
        
        // Creating the resource (i.e: View)
        m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsvDesc, GetDepthStencilBufferDescriptor());

        // Transition the resource from it's initial state to be used as depth stencil buffer
        CD3DX12_RESOURCE_BARRIER transitionedDepthStencilBuffer = CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_d3dCommandList->ResourceBarrier(1, &transitionedDepthStencilBuffer);
    }



#ifdef GUI_ENABLE
    void DX12Renderer::Impl::CreateSrvHeap()
    {
        if (!m_srvHeap)
            m_srvHeap = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE, SRDescriptors::SRCount);
    }

    void DX12Renderer::Impl::CreateRenderTexture()
    {
        if(!m_renderTexture)
            m_renderTexture = std::make_unique<RenderTexture>(m_backBufferFormat);

        m_renderTexture->SetDevice(m_d3dDevice.Get(), m_srvHeap->GetCPUHandleAt(SRDescriptors::SRRenderTexture),
            m_rtvHeap->GetCPUHandleAt(RTDescriptors::RTRenderTexture));

        m_renderTexture->SetClearColor({ 1.0f,0.0f,0.0f,1.0f });

        m_renderTexture->SizeResources(m_window->GetWidth(), m_window->GetHeight(), m_d3dCommandList.Get());

        m_gpuDescriptorHeapManager->CopyDescriptor(m_d3dDevice.Get(), 0, m_srvHeap.get(),
            1);
    }

#endif

    DX12Renderer* DX12Renderer::Impl::m_dx12Renderer = nullptr;

    DX12Renderer::DX12Renderer() : m_impl(std::make_unique<Impl>())
    {
        // Only one instance of this class can be created
        if (m_impl->m_dx12Renderer == nullptr)
        {
            m_impl->m_dx12Renderer = this;
            Logger::Get()->Log(LogType::Info, "Instantiated DX12Renderer");
        }
        else
        {
            THROW_EXCEPTION(3, "DX12Renderer is a singleton class, more than one instance of this class cannot be created")
        }
        
    }

    DX12Renderer::~DX12Renderer()
    {
        
    }

    void DX12Renderer::Initialize(IWindow* window)
    {
        SetTo(window);

        // Initialize Direct3D
        m_impl->InitDirect3D();


        DXThrowIfFailed(m_impl->m_d3dCommandList->Reset(m_impl->m_d3dDirectCmdAllocator.Get(), nullptr));

        // Create CBV heap
        m_impl->CreateCbvHeapAndView();

        // Create root signature
        m_impl->CreateRootSignature();

        m_impl->CreateInputLayout();
        m_impl->CompileShaders();
        m_impl->CreateVertexAndIndexBuffers();

        m_impl->CreatePSO();


        // Execute the resize commands
        DXThrowIfFailed(m_impl->m_d3dCommandList->Close());
        ID3D12CommandList* commands[] = { m_impl->m_d3dCommandList.Get() };
        m_impl->m_d3dCommandQueue->ExecuteCommandLists(_countof(commands), commands);

        // Wait until resize is complete
        m_impl->FlushCommandQueue();

#ifdef GUI_ENABLE
        m_impl->m_gui->Initialize(window, this);
#endif

    }

    void DX12Renderer::SetTo(IWindow *window)
    {
        m_impl->m_window = window;
        m_impl->m_hwnd = m_impl->m_window->GetHandle();

    }

    void DX12Renderer::BeginRender()
    {
        // Reset the command list allocator and reuse the memory associated with it
		// We can only reset after all the command queue commands has been executed
        DXThrowIfFailed(m_impl->m_d3dDirectCmdAllocator->Reset());

        // Reset the Command list, we must close command list before resetting.
        DXThrowIfFailed(m_impl->m_d3dCommandList->Reset(m_impl->m_d3dDirectCmdAllocator.Get(), m_impl->m_pso.Get()));



        // Set the view port, this needs to be reset everytime after the command list is reset
        m_impl->m_d3dCommandList->RSSetViewports(1, &m_impl->m_viewport);
        // Set the scissor rect
        m_impl->m_d3dCommandList->RSSetScissorRects(1, &m_impl->m_scissorRect);

    }

    void DX12Renderer::ClearScreen(float r, float g, float b, float a)
    {
        FLOAT clearColor[4] = { r,g,b,a };


        D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor = m_impl->m_rtvHeap->GetCPUHandleAt(m_impl->m_currentBackBuffer);

    	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_impl->GetDepthStencilBufferDescriptor();


        CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_impl->GetCurrentBackBuffer(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

        m_impl->m_d3dCommandList->ResourceBarrier(1, &transition);


        // Clear the back buffer and depth buffer
        m_impl->m_d3dCommandList->ClearRenderTargetView(rtvDescriptor, clearColor, 0, nullptr);


        m_impl->m_d3dCommandList->ClearDepthStencilView(dsvHandle,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

        // Specify the buffers we are going to render to
        m_impl->m_d3dCommandList->OMSetRenderTargets(1, &rtvDescriptor,
            TRUE, &dsvHandle);



#ifdef GUI_ENABLE


        m_impl->m_gpuDescriptorHeapManager->CopyDescriptor(m_impl->m_d3dDevice.Get(), 2, m_impl->m_cbvHeap.get(),
            m_impl->m_cbvHeap->Count());


        ID3D12DescriptorHeap* descriptorHeaps[] = { m_impl->m_cbvSrvUavGpuHeap->Heap().Get() };

        m_impl->m_d3dCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

        D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRTVHandle = m_impl->m_rtvHeap->GetCPUHandleAt(RTRenderTexture);

        D3D12_CPU_DESCRIPTOR_HANDLE renderTextureDSVHandle = m_impl->m_dsvHeap->GetFirstCPUHandle();


        float* color = m_impl->m_gui->GetClearColor();
        FLOAT rtClearColor[4] = {color[0],color[1],color[2],color[3]};

        m_impl->m_renderTexture->BeginScene(m_impl->m_d3dCommandList.Get());


        m_impl->m_d3dCommandList->OMSetRenderTargets(1, &renderTextureRTVHandle,
            TRUE, &renderTextureDSVHandle);

        //m_impl->m_d3dCommandList->ClearRenderTargetView(renderTextureRTVHandle, rtClearColor, 0, nullptr);
        m_impl->m_renderTexture->Clear(m_impl->m_d3dCommandList.Get());

        m_impl->m_d3dCommandList->ClearDepthStencilView(renderTextureDSVHandle,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);


        m_impl->m_d3dCommandList->SetGraphicsRootSignature(m_impl->m_rootSignature.Get());

        m_impl->m_d3dCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D12_INDEX_BUFFER_VIEW ibView = m_impl->m_geometry->IndexBufferView();
        D3D12_VERTEX_BUFFER_VIEW vbView = m_impl->m_geometry->VertexBufferView();

        m_impl->m_d3dCommandList->IASetIndexBuffer(&ibView);
        m_impl->m_d3dCommandList->IASetVertexBuffers(0, 1, &vbView);

        m_impl->m_d3dCommandList->SetGraphicsRootDescriptorTable(0, m_impl->m_cbvSrvUavGpuHeap->GetGPUHandleAt(2));


        m_impl->m_d3dCommandList->DrawIndexedInstanced(m_impl->m_geometry->SubMeshMap["Box"].IndexCount,
            1, 0, 0, 0);


#endif 


    }

    void DX12Renderer::Update()
    {
        float x = m_impl->m_radius * sinf(m_impl->m_phi) * cosf(m_impl->m_theta);
        float y = m_impl->m_radius * cosf(m_impl->m_phi);
        float z = m_impl->m_radius * sinf(m_impl->m_phi) * sinf(m_impl->m_theta);

        XMMATRIX view = XMMatrixLookAtLH({ x,y,z,1.0f },
            XMVectorZero(), { 0.f,1.0f,0.0f,1.0f });

        XMStoreFloat4x4(&m_impl->m_viewMat, view);

        XMStoreFloat4x4(&m_impl->m_worldMat, XMMatrixIdentity());

        XMMATRIX world = XMLoadFloat4x4(&m_impl->m_worldMat);
        XMMATRIX proj = XMLoadFloat4x4(&m_impl->m_projMat);
        XMMATRIX worldViewProj = world * view * proj;

        ObjectConstants objConstants;


        XMStoreFloat4x4(&objConstants.WorldViewProjMat, XMMatrixTranspose(worldViewProj));
        m_impl->m_cbPerObject->CopyData(0, objConstants);
        m_impl->m_theta += 0.01f;
    }

    void DX12Renderer::EndRender()
    {

#ifdef GUI_ENABLE

        m_impl->m_renderTexture->EndScene(m_impl->m_d3dCommandList.Get());

        D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor = m_impl->m_rtvHeap->GetCPUHandleAt(m_impl->m_currentBackBuffer);

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_impl->GetDepthStencilBufferDescriptor();

        m_impl->m_d3dCommandList->OMSetRenderTargets(1, &rtvDescriptor,
            TRUE, &dsvHandle);


        ImTextureID texture = reinterpret_cast<ImTextureID>(m_impl->m_cbvSrvUavGpuHeap->GetGPUHandleAt(SRRenderTexture).ptr);

        m_impl->m_gui->BeginDraw();

        
        m_impl->m_gui->DrawToCentral(texture);

        std::stringstream ssr;

        ssr << "Window width: " << m_impl->m_window->GetWidth() << ", " << "Window height: " << m_impl->m_window->GetHeight() << "\n"
            << "Render Texture width: " << m_impl->m_renderTexture->GetWidth() << ", " << "Render Texture height: " << m_impl->m_renderTexture->GetHeight();
        	

        m_impl->m_gui->WriteToConsole(ssr.str());

        m_impl->m_gui->EndDraw();

#endif


        CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_impl->GetCurrentBackBuffer(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

        m_impl->m_d3dCommandList->ResourceBarrier(1, &transition);


        // Done recording commands
        DXThrowIfFailed(m_impl->m_d3dCommandList->Close());

        // add the command list to the queue for execution
        ID3D12CommandList* commands[] = { m_impl->m_d3dCommandList.Get() };
        m_impl->m_d3dCommandQueue->ExecuteCommandLists(_countof(commands), commands);

        // Swap the back and front buffer
        DXThrowIfFailed(m_impl->m_dxgiSwapChain->Present(0, 0));
        m_impl->m_currentBackBuffer = (m_impl->m_currentBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

        // Wait until frame commands are complete
        m_impl->FlushCommandQueue();

      //  m_impl->m_gpuDescriptorHeapManager->Reset();

    }

    void DX12Renderer::Close()
    {
        Logger::Get()->Log(LogType::Info, "Closing DirectX12 Renderer");

        // Wait for GPU to finish before destroying
        if (m_impl->m_d3dDevice != nullptr)
            m_impl->FlushCommandQueue();

#if defined(DEBUG) || defined(_DEBUG)
        Logger::Get()->Log(LogType::Info, "Creating DXGI debug object and reporting live objects");

        ComPtr<IDXGIDebug1> pdxgiDebug{ nullptr };
        DXThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(pdxgiDebug.ReleaseAndGetAddressOf())));
        DXThrowIfFailed(pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL));
#endif

#ifdef GUI_ENABLE
        m_impl->m_gui->Close();
#endif


    }

    void DX12Renderer::Resize()
    {
        m_impl->OnResize();

        float aspectRatio = m_impl->m_gui->GetCentralNodeSize().x / m_impl->m_gui->GetCentralNodeSize().y;//static_cast<float>(m_impl->m_window->GetWidth()) / m_impl->m_window->GetHeight();

        XMMATRIX p = XMMatrixPerspectiveFovLH(0.25f * XM_PI, aspectRatio , 1.0f, 1000.f);
        XMStoreFloat4x4(&m_impl->m_projMat, p);
    }

    ID3D12Device* DX12Renderer::GetD3DDevice() const
    {
        return m_impl->m_d3dDevice.Get();
    }

    DXGI_FORMAT DX12Renderer::GetRTVFormat() const
    {
        return m_impl->m_backBufferFormat;
    }

    ID3D12GraphicsCommandList* DX12Renderer::GetCommandList() const
    {
        return m_impl->m_d3dCommandList.Get();
    }

    DescriptorHeap* DX12Renderer::GetSrvHeap() const
    {
        return m_impl->m_cbvSrvUavGpuHeap.get();
    }

    void DX12Renderer::CheckCPUDescMan()
    {
        CpuDescriptorHeap cpuHeap(m_impl->m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        {
            auto allocation1 = cpuHeap.Allocate(20);
            //cpuHeap.Free(std::move(allocation1));
            auto allocation2 = cpuHeap.Allocate(100);
        }

        cpuHeap.ReleaseStaleAllocations(2);

        GpuDescriptorHeap gpuHeap(m_impl->m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        DynamicSubAllocationManager dynamicGpuHeap(&gpuHeap);

        {
            auto allocation3 = gpuHeap.Allocate(20);
            auto allocation4 = gpuHeap.Allocate(100);
        }
        {
            auto allocation5 = dynamicGpuHeap.Allocate(20);
            auto allocation6 = dynamicGpuHeap.Allocate(100);
            auto allocation7 = dynamicGpuHeap.Allocate(2);

            auto allocation8 = gpuHeap.AllocateDynamic(10);
            auto allocation9 = gpuHeap.AllocateDynamic(5);
            gpuHeap.Free(std::move(allocation8));
        }

        dynamicGpuHeap.DiscardAllocations(2);

        gpuHeap.ReleaseStaleAllocations(2);

        {
            dynamicGpuHeap.Allocate(20);
            dynamicGpuHeap.Allocate(100);
            dynamicGpuHeap.Allocate(2);

            dynamicGpuHeap.DiscardAllocations(2);
        }

    }


    void DX12Renderer::Impl::InitDirect3D()
    {
        // enable D3D debug layer if DEBUG mode is enabled
#if defined(DEBUG) || defined(_DEBUG)

        ComPtr<ID3D12Debug6> pD3DDebug;
        DXThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(pD3DDebug.ReleaseAndGetAddressOf())));
        pD3DDebug.As(&m_d3dDebug);
        m_d3dDebug->EnableDebugLayer();
        
        Logger::Get()->Log(LogType::Info, "Enabled D3D Debug Layer");

#endif

        InitDXGI();

        Logger::Get()->Log(LogType::Info, "Creating D3D12 Device");
        HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));

        // Query for maximum feature level support

        D3D_FEATURE_LEVEL featureLevels[5] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_2
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo = { 0 };
        featureLevelInfo.NumFeatureLevels = 5;
        featureLevelInfo.pFeatureLevelsRequested = featureLevels;

        DXThrowIfFailed(m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

        D3D_FEATURE_LEVEL maximumFeatureLevel = featureLevelInfo.MaxSupportedFeatureLevel;

        Logger::Get()->Log(LogType::Info, "Max supported feature level: " + std::to_string(maximumFeatureLevel));

        Logger::Get()->Log(LogType::Info, "Discarding old D3D Device and creating new one with Maximum feature level support");

        m_d3dDevice.Reset();

        hr = D3D12CreateDevice(nullptr, maximumFeatureLevel, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf()));


        if(FAILED(hr))
        {
            Logger::Get()->Log(LogType::Info, "Creating D3D12 Failed, falling back to warp adapter");

            ComPtr<IDXGIAdapter> pWarpAdapter{ nullptr };
            DXThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(pWarpAdapter.ReleaseAndGetAddressOf())));

            DXThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf())));
        }

        DXThrowIfFailed(hr);

        Logger::Get()->Log(LogType::Info, "Creating D3D12 Fence object");
        DXThrowIfFailed(m_d3dDevice->CreateFence(m_currentFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));

        Logger::Get()->Log(LogType::Info, "Caching Descriptor Handle Increment Size");

        m_RtvDHandleIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_DsvDHandleIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        m_CbvSrvUavDHandleIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        m_SamplerDHandleIncrementSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

        Logger::Get()->Buffer() << "RTV handle increment size : " + std::to_string(m_RtvDHandleIncrementSize) + "\n" +
						            "DSV handle increment size : " + std::to_string(m_DsvDHandleIncrementSize) + "\n" +
						            "CBV / SRV / UAV handle increment size: " + std::to_string(m_CbvSrvUavDHandleIncrementSize) + "\n" +
						            "Sampler handle increment size: " + std::to_string(m_SamplerDHandleIncrementSize) + "\n";
        Logger::Get()->Flush();

        
        // Checking support for MSAA
        m_MSAAQuality = CheckMSAASupport(m_d3dDevice.Get(), m_MSAASampleCount, m_backBufferFormat);

        // Create Command objects
        CreateCommandObjects(m_d3dDevice.Get(), m_d3dCommandQueue, m_d3dDirectCmdAllocator,
            m_d3dCommandList);


        // Create Swapchain
        ComPtr<IDXGISwapChain1> pdxgiSwapChain1{ nullptr };
        CreateSwapChain(m_dxgiFactory.Get(), pdxgiSwapChain1.ReleaseAndGetAddressOf(), m_d3dCommandQueue.Get(), m_backBufferFormat, m_swapChainBufferCount);
        pdxgiSwapChain1.As(&m_dxgiSwapChain);

        
        // Enum adapters
        m_dxgiAdapters = EnumAdapters(m_dxgiFactory.Get());

        // Create RTV and DSV heap
        CreateRtvHeap();
        CreateDsvHeap();

        if (!m_cbvSrvUavGpuHeap)
        {
            m_cbvSrvUavGpuHeap = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 128);

            m_gpuDescriptorHeapManager = std::make_unique<GPUDescriptorHeapManager>(m_cbvSrvUavGpuHeap.get());
        }


#ifdef GUI_ENABLE
        CreateSrvHeap();

        m_gui = std::make_unique<DX12Imgui>();

#endif

        OnResize();
    }

    void DX12Renderer::Impl::InitDXGI()
    {
        Logger::Get()->Log(LogType::Info, "Initializing DXGI and created DXGI Factory");

        DWORD dxgiFactoryFlags = 0;
#ifdef _DEBUG
        dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

        DXThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags,IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
    }

    UINT DX12Renderer::Impl::CheckMSAASupport(ID3D12Device* pd3dDevice, UINT sampleCount, DXGI_FORMAT backBufferFormat)
    {
        Logger::Get()->Log(LogType::Info, "Checking MSAA Support for Sample count: " + std::to_string(sampleCount));

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQualityLevels;
        msaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        msaaQualityLevels.Format = backBufferFormat;
        msaaQualityLevels.SampleCount = sampleCount;
        msaaQualityLevels.NumQualityLevels = 0;

        DXThrowIfFailed(pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaQualityLevels,
            sizeof(msaaQualityLevels)));

        UINT qualityLevel = msaaQualityLevels.NumQualityLevels;

        if (qualityLevel < 0)
            THROW_EXCEPTION(3, "Unexpected MSAA quality level");

        Logger::Get()->Buffer() << "Supported MSAA quality level : " + std::to_string(qualityLevel) + "\n";
        Logger::Get()->Flush();

        return qualityLevel;
    }

    void DX12Renderer::Impl::CreateCommandObjects(ID3D12Device* pd3dDevice, ComPtr<ID3D12CommandQueue>& pCmdQueue, ComPtr<ID3D12CommandAllocator>& pCmdAllocator, ComPtr<ID3D12GraphicsCommandList>& pCmdList)
    {
        Logger::Get()->Log(LogType::Info, "Creating Command Objects");


        D3D12_COMMAND_QUEUE_DESC commandQDesc = {};
        commandQDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        commandQDesc.NodeMask = 0;
        commandQDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        // Creating the Command queue
        DXThrowIfFailed(pd3dDevice->CreateCommandQueue(&commandQDesc, IID_PPV_ARGS(&pCmdQueue)));

        // Creating the command allocator
        DXThrowIfFailed(pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator)));

        // Creating the command list
        DXThrowIfFailed(pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator.Get(),
            nullptr, IID_PPV_ARGS(&pCmdList)));


        // Close the command list for later reset
        pCmdList->Close();
    }

    void DX12Renderer::Impl::CreateSwapChain(IDXGIFactory4* pDXGIFactory, IDXGISwapChain1** pSwapChain, ID3D12CommandQueue* pCommandQueue, DXGI_FORMAT backBufferFormat, UINT bufferCount)
    {
        Logger::Get()->Log(LogType::Info, "Creating DXGI SwapChain");

        // Release the SwapChain, this allows us to discard the old SwapChain and create new in runtime
        if (*pSwapChain)
            (*pSwapChain)->Release();

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = m_window->GetWidth();
        swapChainDesc.Height = m_window->GetHeight();
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = GetMSAAState() ? GetMSAASampleCount() : 1;
        swapChainDesc.SampleDesc.Quality = GetMSAAState() ? (GetMSAAQuality() - 1) : 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = bufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFSDesc = {};
        swapChainFSDesc.Windowed = TRUE;



        // Swapchain uses queue to perform flush
        DXThrowIfFailed(pDXGIFactory->CreateSwapChainForHwnd(pCommandQueue,m_hwnd, &swapChainDesc, &swapChainFSDesc, nullptr, pSwapChain));

    }

    std::vector<ComPtr<IDXGIAdapter>> DX12Renderer::Impl::EnumAdapters(IDXGIFactory* pDXGIfactory)
    {
        Logger::Get()->Log(LogType::Info, "Logging Display Adapters");


        UINT i = 0;
        ComPtr<IDXGIAdapter> pAdapter{ nullptr };
        std::vector<ComPtr<IDXGIAdapter>> adapterList;

        while(pDXGIfactory->EnumAdapters(i,&pAdapter) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC adapterDesc;
            pAdapter->GetDesc(&adapterDesc);

            std::wstring adapterName = adapterDesc.Description;
            SIZE_T dedicatedSysMem = (adapterDesc.DedicatedSystemMemory == 0) ? adapterDesc.DedicatedSystemMemory : (adapterDesc.DedicatedSystemMemory / 1024) / 1024;
            SIZE_T dedicatedVidMem = (adapterDesc.DedicatedVideoMemory / 1024) / 1024;
            SIZE_T sharedSysMem = (adapterDesc.SharedSystemMemory / 1024) / 1024;
            UINT   deviceId = adapterDesc.DeviceId;
            UINT   vendorId = adapterDesc.VendorId;
            UINT   subSysId = adapterDesc.SubSysId;
            UINT   revision = adapterDesc.Revision;

            std::wstring text = L"*****		ADAPTER	DESCRIPTION		*****\n";
            text += L"ADAPTER INDEX				: " + std::to_wstring(i + 1) + L"\n";
            text += L"NAME						: " + adapterName + L"\n";
            text += L"DEDICATED SYSTEM MEMORY	: " + std::to_wstring(dedicatedSysMem) + L" MB\n";
            text += L"DEDICATED VIDEO MEMORY	: " + std::to_wstring(dedicatedVidMem) + L" MB\n";
            text += L"SHARED SYSTEM MEMORY		: " + std::to_wstring(sharedSysMem) + L" MB\n";
            text += L"DEVICE ID					: " + std::to_wstring(deviceId) + L"\n";
            text += L"VENDOR ID					: " + std::to_wstring(vendorId) + L"\n";
            text += L"SUB-SYSTEM ID				: " + std::to_wstring(subSysId) + L"\n";
            text += L"REVISION					: " + std::to_wstring(revision) + L"\n";
            text += L"	**********************************************	\n";

            Logger::Get()->Buffer() << WstringToString(text);

            adapterList.push_back(pAdapter);

            ++i;
        }

      //  Logger::Get()->Flush();

        for (size_t i = 0; i < adapterList.size(); i++)
        {
            EnumAdapterOutputs(adapterList[i].Get());
        }

        return adapterList;
    }

    void DX12Renderer::Impl::EnumAdapterOutputs(IDXGIAdapter* pAdapter)
    {
        Logger::Get()->Log(LogType::Info, "Logging Adapter Outputs");

        IDXGIOutput* pOutput{ nullptr };
        UINT i{ 0 };

        while (pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_OUTPUT_DESC desc = { 0 };
            pOutput->GetDesc(&desc);

            std::wstring deviceName = desc.DeviceName;
            RECT desktopCoor = desc.DesktopCoordinates;

            std::wstring outputCoordStr = L"LEFT: " + std::to_wstring(desktopCoor.left) +
                L" RIGHT: " + std::to_wstring(desktopCoor.right) +
                L" TOP: " + std::to_wstring(desktopCoor.top) +
                L" BOTTOM: " + std::to_wstring(desktopCoor.bottom);

            std::wstring text = L"\n*****		OUTPUT	DESCRIPTION		*****\n";
            text += L"OUTPUT INDEX			    : " + std::to_wstring(i + 1) + L"\n";
            text += L"OUTPUT NAME				: " + deviceName + L"\n";
            text += L"OUTPUT COORDINATES		: " + outputCoordStr + L"\n";
            text += L"SUPPORTED OUTPUT MODES	:\n";

            Logger::Get()->Buffer() << WstringToString(text);
            Logger::Get()->Flush();

            EnumOutputDisplayModes(pOutput, m_backBufferFormat);

            DXSafe_Release(pOutput);

            ++i;
        }
    }

    void DX12Renderer::Impl::EnumOutputDisplayModes(IDXGIOutput* pOutput, DXGI_FORMAT backBufferFormat)
    {

        Logger::Get()->Log(LogType::Info, "Logging Adapter Outputs Display Modes");


        UINT count{ 0 };
        UINT flags{ 0 };

        // Calling with nullptr to get the list count.
        pOutput->GetDisplayModeList(backBufferFormat, flags, &count, nullptr);

        std::vector<DXGI_MODE_DESC> modeList(count);

        pOutput->GetDisplayModeList(backBufferFormat, flags, &count, &modeList[0]);

        UINT i{ 1 };
        for (auto& x : modeList)
        {
            UINT n = x.RefreshRate.Numerator;
            UINT d = x.RefreshRate.Denominator;
            float rt = static_cast<float>(n) / static_cast<float> (d);

            std::wstring text =
                std::to_wstring(i) + L". WIDTH = " + std::to_wstring(x.Width) + L" " +
                L"HEIGHT = " + std::to_wstring(x.Height) + L" " +
                L"REFRESH RATE = " + std::to_wstring(n) + L"/" + std::to_wstring(d) + L" = " + std::to_wstring(rt) + L" HZ"
                L"\n";

            Logger::Get()->Buffer() << WstringToString(text);
            Logger::Get()->Flush();
           
            ++i;
        }
    }

    void DX12Renderer::Impl::CreateRootSignature()
    {
        
        CD3DX12_ROOT_PARAMETER    slotRootParameter[2];

        // Root parameters can be Descriptor table, Root constant or Root descriptor
        CD3DX12_DESCRIPTOR_RANGE    cbvTable;
    	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

        // Setting this root parameter as a descriptor table
        slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
      
        // Root signature is just an array of root parameters
        CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3DBlob>    serializedRootSig{ nullptr };
        ComPtr<ID3DBlob>    errorBlob{ nullptr };

        DXThrowIfFailed(D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
            serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf()));

        assert(errorBlob == nullptr);

        DXThrowIfFailed(m_d3dDevice->CreateRootSignature(0,
            serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(),
            IID_PPV_ARGS(&m_rootSignature)));

    }

    void DX12Renderer::Impl::CreateInputLayout()
    {

        D3D12_INPUT_ELEMENT_DESC    inputElements[2];

        D3D12_INPUT_ELEMENT_DESC    posElementDesc = {};
        posElementDesc.SemanticName = "POSITION";
        posElementDesc.SemanticIndex = 0;
        posElementDesc.AlignedByteOffset = 0;
        posElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        posElementDesc.InputSlot = 0;
        posElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        posElementDesc.InstanceDataStepRate = 0;

        D3D12_INPUT_ELEMENT_DESC    colorElementDesc = {};
        colorElementDesc.SemanticName = "COLOR";
        colorElementDesc.SemanticIndex = 0;
        colorElementDesc.AlignedByteOffset = 12;
        colorElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        colorElementDesc.InputSlot = 0;
        colorElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

        inputElements[0] = posElementDesc;
        inputElements[1] = colorElementDesc;

        m_inputElements =
        {
            {
                "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
            },

            {
                "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
            }
        };

    }


    void DX12Renderer::Impl::CreateViewportAndScissorRect()
    {
        // Viewport
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;
        m_viewport.Width = static_cast<FLOAT>(m_window->GetWidth());
        m_viewport.Height = static_cast<FLOAT>(m_window->GetHeight());
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;

        
        // Scissor rect
        m_scissorRect.left = 0;
        m_scissorRect.top = 0;
        m_scissorRect.right = static_cast<LONG>(m_window->GetWidth());
        m_scissorRect.bottom = static_cast<LONG>(m_window->GetHeight());
    }

    void DX12Renderer::Impl::FlushCommandQueue()
    {
        // Increase the current fence value by 1
        m_currentFenceValue++;

        // Add instruction to command queue to set a new fence point
        DXThrowIfFailed(m_d3dCommandQueue->Signal(m_fence.Get(), m_currentFenceValue));

        // Check to see if the Fence has reached the current Fence value
        if(m_fence->GetCompletedValue() < m_currentFenceValue)
        {
           HANDLE eventHandle =  CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

            // Fire event when GPU hits current fence
           DXThrowIfFailed(m_fence->SetEventOnCompletion(m_currentFenceValue, eventHandle));

            // Wait until GPU hits current fence point
           WaitForSingleObject(eventHandle, INFINITE);

           CloseHandle(eventHandle);
        }

    }

    void DX12Renderer::Impl::OnResize()
    {
        Logger::Get()->Log(LogType::Info, "Buffers Resizing");

        assert(m_d3dDevice);
        assert(m_dxgiSwapChain);
        assert(m_d3dDirectCmdAllocator);

        // Wait for GPU before changing any resources
        FlushCommandQueue();

        DXThrowIfFailed(m_d3dCommandList->Reset(m_d3dDirectCmdAllocator.Get(), nullptr));

        // Release the resources we'll be recreating
        for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
        {
            m_swapChainBuffers[i].Reset();
        }
        m_depthStencilBuffer.Reset();

        // Resize the SwapChain
        DXThrowIfFailed(m_dxgiSwapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, m_window->GetWidth(), m_window->GetHeight(),
            m_backBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

        m_currentBackBuffer = 0;

        // Get the Render target buffer from SwapChain and create Descriptor
        CreateRtv();


#ifdef  GUI_ENABLE
        if (m_renderTexture)
            m_renderTexture->ReleaseDevice();
        CreateRenderTexture();

#endif
        // Create the depth stencil buffer and Descriptor
        CreateDsv(m_window->GetHeight(), m_window->GetWidth());
       

        // Execute the resize commands
        DXThrowIfFailed(m_d3dCommandList->Close());
        ID3D12CommandList* commands[] = { m_d3dCommandList.Get() };
        m_d3dCommandQueue->ExecuteCommandLists(_countof(commands), commands);

        // Wait until resize is complete
        FlushCommandQueue();

        // Update the viewport and scissor rect
        CreateViewportAndScissorRect();

    }

    bool DX12Renderer::Impl::GetMSAAState() const
    {
        return m_isMSAAEnabled;
    }

    UINT DX12Renderer::Impl::GetMSAASampleCount() const
    {
        return m_MSAASampleCount;
    }

    UINT DX12Renderer::Impl::GetMSAAQuality() const
    {
        return m_MSAAQuality;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Renderer::Impl::GetCurrentBackBufferCPUDescriptor() const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetFirstCPUHandle(), static_cast<INT>(m_currentBackBuffer), m_RtvDHandleIncrementSize);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Renderer::Impl::GetDepthStencilBufferDescriptor() const
    {
        return m_dsvHeap->GetFirstCPUHandle();
    }

    ID3D12Resource* DX12Renderer::Impl::GetCurrentBackBuffer() const
    {
        return m_swapChainBuffers[m_currentBackBuffer].Get();
    }

    ID3D12Resource* DX12Renderer::Impl::GetDepthStencilBuffer() const
    {
        return m_depthStencilBuffer.Get();
    }

} // BINDU