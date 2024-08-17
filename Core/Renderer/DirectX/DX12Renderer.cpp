//
// Created by letsd on 13-Aug-24.
//

#include "DX12Renderer.h"

#include <assert.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <sstream>
#include <string>

#include "../../../Logger/Logger.h"
#include "../../../Utility/Common/CommonUtility.h"
#include "Utility/D3DUtillity.h"
#include "Utility/d3dx12.h"


namespace BINDU {

    using namespace Microsoft::WRL;


    class DX12Renderer::Impl
    {
    public:
        // Pointer to the DX12Renderer class
        static DX12Renderer* m_dx12Renderer;

        // Methods
        void InitDirect3D();

        void InitDXGI();

        UINT CheckMSAASupport(ID3D12Device* pd3dDevice, UINT sampleCount, DXGI_FORMAT backBufferFormat);

        void CreateCommandObjects(ID3D12Device* pd3dDevice, ComPtr<ID3D12CommandQueue>& pCmdQueue, ComPtr<ID3D12CommandAllocator> pCmdAllocator, ComPtr<ID3D12GraphicsCommandList> pCmdList);

        void CreateSwapChain(IDXGIFactory4* pDXGIFactory, IDXGISwapChain1** pSwapChain, ID3D12CommandQueue* pCommandQueue, DXGI_FORMAT backBufferFormat, UINT bufferCount);

        std::vector<ComPtr<IDXGIAdapter>> EnumAdapters(IDXGIFactory* pDXGIfactory);
        void EnumAdapterOutputs(IDXGIAdapter* pAdapter);
        void EnumOutputDisplayModes(IDXGIOutput* pOutput, DXGI_FORMAT backBufferFormat);

        void CreateRTVandDSVHeap();
        void CreateRTVDescriptors();
        void CreateDSVDescriptor();

        void CreateViewportAndScissorRect();

        // Getters
        bool GetMSAAState() const;
        UINT GetMSAASampleCount() const;
        UINT GetMSAAQuality() const;

        // returns cpu handle to the current Back buffer
        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferHandle() const;

        // return handle to the Depth Stencil buffer
        D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilBufferHandle() const;

        // Variables


        // DXGI Stuff

        // DXGI Available adapters
        std::vector<ComPtr<IDXGIAdapter>> m_dxgiAdapters;

        // DXGI Factory
        ComPtr<IDXGIFactory4>    m_dxgiFactory{ nullptr };

        // DXGI SwapChain
        ComPtr<IDXGISwapChain1>  m_dxgiSwapChain{ nullptr };

        // SwapChain Buffers
        UINT m_swapChainBufferCount = SwapChainBufferCount;

        ComPtr<ID3D12Resource> m_swapChainBuffers[SwapChainBufferCount];

        UINT m_currentBackBuffer = 0;


        // Direct3D Stuff

        // Direct3D Device
        ComPtr<ID3D12Device>    m_d3dDevice{ nullptr };

        // Direct3D Debug interface
        ComPtr<ID3D12Debug6>    m_d3dDebug{ nullptr };

        // D3D Fence
        UINT64                  m_currentFence = 0;
        ComPtr<ID3D12Fence>     m_fence{ nullptr };

        // D3D Command Objects

        // D3D Command Queue
        ComPtr<ID3D12CommandQueue> m_d3dCommandQueue{ nullptr };
        // D3D Command Allocator
        ComPtr<ID3D12CommandAllocator> m_d3dDirectCmdAllocator{ nullptr };
        // D3D Graphics Command List
        ComPtr<ID3D12GraphicsCommandList> m_d3dCommandList{ nullptr };

        // Descriptor heaps

    	// RTV heap
        ComPtr<ID3D12DescriptorHeap>    m_rtvHeap{ nullptr };
        // DSV heap
        ComPtr<ID3D12DescriptorHeap>    m_dsvHeap{ nullptr };

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

    };


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

        m_impl->InitDirect3D();

    }

    void DX12Renderer::SetTo(IWindow *window)
    {
        m_impl->m_window = window;
        m_impl->m_hwnd = m_impl->m_window->GetHandle();

    }

    void DX12Renderer::Close()
    {

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
        DXThrowIfFailed(m_d3dDevice->CreateFence(m_currentFence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));

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
        CreateSwapChain(m_dxgiFactory.Get(), m_dxgiSwapChain.ReleaseAndGetAddressOf(), m_d3dCommandQueue.Get(), m_backBufferFormat, m_swapChainBufferCount);

        // Enum adapters
        m_dxgiAdapters = EnumAdapters(m_dxgiFactory.Get());
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

        Logger::Get()->Buffer() << "Supported MSAA quality level : " + std::to_string(qualityLevel);
        Logger::Get()->Flush();

        return qualityLevel;
    }

    void DX12Renderer::Impl::CreateCommandObjects(ID3D12Device* pd3dDevice, ComPtr<ID3D12CommandQueue>& pCmdQueue, ComPtr<ID3D12CommandAllocator> pCmdAllocator, ComPtr<ID3D12GraphicsCommandList> pCmdList)
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

    void DX12Renderer::Impl::CreateRTVandDSVHeap()
    {

        Logger::Get()->Log(LogType::Info, "Creating RTV heap");

        // Create the RTV heap
       DXThrowIfFailed(D3DUtility::CreateDescriptorHeap(m_d3dDevice.Get(), m_swapChainBufferCount,
           D3D12_DESCRIPTOR_HEAP_TYPE_RTV, m_rtvHeap.ReleaseAndGetAddressOf()));

       Logger::Get()->Log(LogType::Info, "Creating DSV heap");

       // Create the DSV heap
       DXThrowIfFailed(D3DUtility::CreateDescriptorHeap(m_d3dDevice.Get(), 1,
           D3D12_DESCRIPTOR_HEAP_TYPE_DSV, m_dsvHeap.ReleaseAndGetAddressOf()));
    }

    void DX12Renderer::Impl::CreateRTVDescriptors()
    {
        
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        for(UINT i = 0 ; i<SwapChainBufferCount; i++)
        {
            // Get the ith buffer from the SwapChain
            m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(m_swapChainBuffers[i].ReleaseAndGetAddressOf()));

            // Create a render target to that buffer
            m_d3dDevice->CreateRenderTargetView(m_swapChainBuffers[i].Get(), nullptr, rtvHandle);

            // Offset the handle by 1 to get to the next rtv handle
            rtvHandle.Offset(1, m_RtvDHandleIncrementSize);
        }
    }

    void DX12Renderer::Impl::CreateDSVDescriptor()
    {
        Logger::Get()->Log(LogType::Info, "Creating depth stencil buffer and descriptor");

        // Depth stencil buffer is a Texture2D. so, create a resource description for it
        D3D12_RESOURCE_DESC rsd = {};
        rsd.Format = m_depthStencilFormat;
        rsd.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        rsd.Height = m_window->GetHeight();
        rsd.Width = m_window->GetWidth();
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

        // Creating the descriptor (i.e: View)
        m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, GetDepthStencilBufferHandle());

        // Transition the resource from it's initial state to be used as depth stencil buffer
        CD3DX12_RESOURCE_BARRIER transitionedDepthStencilBuffer = CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_d3dCommandList->ResourceBarrier(1, &transitionedDepthStencilBuffer);

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

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Renderer::Impl::GetCurrentBackBufferHandle() const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(m_currentBackBuffer), m_RtvDHandleIncrementSize);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Renderer::Impl::GetDepthStencilBufferHandle() const
    {
        return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
    }

} // BINDU