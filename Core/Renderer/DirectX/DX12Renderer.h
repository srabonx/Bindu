//
// Created by letsd on 13-Aug-24.
//

#ifndef BINDU_DX12RENDERER_H
#define BINDU_DX12RENDERER_H

#include <memory>
#include "../IRenderer.h"

namespace BINDU {

    class DX12Renderer : public IRenderer
    {
    public:
        DX12Renderer();

        DX12Renderer(const DX12Renderer& rhs) = delete;

        DX12Renderer& operator = (const DX12Renderer& rhs) = delete;

        ~DX12Renderer();


        void Initialize(IWindow* window) override;

        void SetTo(IWindow *window) override;

        void ClearScreen(float r, float g, float b, float a) override;

        void Present() override;

        void Close() override;
        

    private:

        class Impl;
        std::unique_ptr<Impl> m_impl {nullptr};
    };

} // BINDU

#endif //BINDU_DX12RENDERER_H
