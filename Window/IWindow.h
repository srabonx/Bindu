//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_IWINDOW_H
#define BINDU_IWINDOW_H

#include <string>

namespace BINDU {

    class EventManager;

    // Base class for all type of Ev_Window
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        virtual void Create(uint16_t width, uint16_t height, std::string title) = 0;

        //Register the Event for this Ev_Window
        virtual void RegisterEventManager(EventManager* eventManager) = 0;
    };

} // BINDU

#endif //BINDU_IWINDOW_H
