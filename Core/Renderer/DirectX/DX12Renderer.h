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

        ~DX12Renderer();

        void Initialize() override;

        void SetTo(IWindow *window) override;

        void Close() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl {nullptr};
    };

} // BINDU

#endif //BINDU_DX12RENDERER_H
