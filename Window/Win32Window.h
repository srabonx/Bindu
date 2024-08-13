//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_WIN32WINDOW_H
#define BINDU_WIN32WINDOW_H

#include <windows.h>
#include <memory>
#include "IWindow.h"


namespace BINDU {

    class Win32Window : public IWindow
    {
    public:
        explicit Win32Window(HINSTANCE hInstance);
        ~Win32Window() override = default;

        void Create(uint16_t width, uint16_t height, std::string title) override;

        HWND& GetHandle() override;

        void RegisterEventManager(EventManager* eventManager) override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_WIN32WINDOW_H
