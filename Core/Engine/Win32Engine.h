//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_WIN32ENGINE_H
#define BINDU_WIN32ENGINE_H

#include <memory>
#include "IEngine.h"

namespace BINDU {

    class IBindu_App;

    class Win32Engine : public IEngine
    {
    public:
        Win32Engine(IBindu_App* app);

        ~Win32Engine() override = default;

        void Initialize() override;

        void Run() override;

        void Close() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_WIN32ENGINE_H
