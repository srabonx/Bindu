//
// Created by letsd on 13-Aug-24.
//

#ifndef BINDU_DX12RENDERER_H
#define BINDU_DX12RENDERER_H

#include <d3d12.h>
#include <memory>
#include "../IRenderer.h"

namespace BINDU {


    enum RTDescriptors
    {
        RTBackBuffer1,
        RTBackBuffer2,
#ifdef GUI_ENABLE
        RTRenderTexture,
#endif
        RTCount
    };

    enum SRDescriptors
    {
        SRRenderTexture,
        SRGui,
        SRCount
    };




    class DescriptorHeap;

    class DX12Renderer : public IRenderer
    {
    public:
        DX12Renderer();

        DX12Renderer(const DX12Renderer& rhs) = delete;

        DX12Renderer& operator = (const DX12Renderer& rhs) = delete;

        ~DX12Renderer();


        void Initialize(IWindow* window) override;

        void SetTo(IWindow *window) override;

        void BeginRender() override;

        void ClearScreen(float r, float g, float b, float a) override;

        void EndRender() override;

        void Close() override;

        void Resize() override;


        // Getters
        ID3D12Device*                   GetD3DDevice() const;
        DXGI_FORMAT                     GetRTVFormat() const;
        ID3D12GraphicsCommandList*      GetCommandList() const;

        DescriptorHeap*                 GetSrvHeap() const;


    private:

        class Impl;
        std::unique_ptr<Impl> m_impl {nullptr};
    };

} // BINDU

#endif //BINDU_DX12RENDERER_H
