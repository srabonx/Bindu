//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_IWINDOW_H
#define BINDU_IWINDOW_H

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include <string>

namespace BINDU {

    class EventManager;

    // Base class for all type of Ev_Window
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        virtual void Create(uint16_t width, uint16_t height, std::string title) = 0;

#if defined(WIN32) || defined(_WIN32)
        virtual HWND& GetHandle() = 0;
#endif

        //Register the Event for this Ev_Window
        virtual void RegisterEventManager(EventManager* eventManager) = 0;

        // Setters
        virtual void SetWindowTitle(const std::string& title) = 0;

        // Getters
        virtual std::string GetWindowTitle() const = 0;
        virtual UINT GetHeight() const = 0;
        virtual UINT GetWidth() const = 0;

    };

} // BINDU

#endif //BINDU_IWINDOW_H
