//
// Created by letsd on 13-Aug-24.
//

#include "DX12Renderer.h"
#include <d3d12.h>
#include <dxgi.h>
#include <wrl.h>
#include "Utility/D3DUtillity.h"


namespace BINDU {

    using namespace Microsoft::WRL;


    class DX12Renderer::Impl
    {
    public:
        // Methods
        void InitDirect3D();

        void InitDXGI();



        // Variables


        // DXGI Stuff

        // DXGI Factory
        ComPtr<IDXGIFactory>    m_dxgiFactory{ nullptr };

        // Direct3D Stuff

        // Direct3D Device
        ComPtr<ID3D12Device>    m_d3dDevice{ nullptr };


    };

    DX12Renderer::DX12Renderer() : m_impl(std::make_unique<Impl>())
    {

    }

    DX12Renderer::~DX12Renderer()
    {

    }

    void DX12Renderer::Initialize()
    {

    }

    void DX12Renderer::SetTo(IWindow *window)
    {

    }

    void DX12Renderer::Close()
    {

    }



    void DX12Renderer::Impl::InitDirect3D()
    {

    }

    void DX12Renderer::Impl::InitDXGI()
    {
        DXThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

    }

} // BINDU