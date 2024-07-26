//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_EVENTSTRUCT_H
#define BINDU_EVENTSTRUCT_H

#include <cstdint>
#include <string>

namespace BINDU::EVENT {

    enum class Type
    {
        NONE = 0,
        CUSTOM,
        QUIT,
        DESTROY,
        WINDOW_ENTER_RESIZING,
        WINDOW_EXIT_RESIZING,
        WINDOW_SIZE_CHANGED,
        WINDOW_INACTIVE,
        WINDOW_ACTIVE,
        WINDOW_MINIMIZED,
        WINDOW_MAXIMIZED,
        WINDOW_RESTORED,            // Restored from maximized or minimized state
        KEY_DOWN,
        KEY_UP,
        MOUSE_DOWN,
        MOUSE_UP,

    };






    struct WindowState
    {
        std::uint16_t Width{0};
        std::uint16_t Height{0};
    };

    /* Custom Event */
    /* can be used to push custom event into the event pool
     * your custom event structure must derive from ICustomEvent to be able to get intercepted.
     * Push the custom event into the event pool using CUSTOM event type.
     * Intercept the custom event later in your EventListener and recast it to your appropriate derived class
     * to get access to your Custom Event Structure.
     */
    struct ICustomEvent
    {
        std::string Name;
    };



    // Different stats relevant to current event
    // Can only have one at a time
    union BND_Event_Stats
    {
        WindowState Window;
        ICustomEvent* CustomEvent{nullptr};
    };



    /* Event Structure */
    /*
     * Holds only the current event type
     * and a relevant stat at a time
     */

    struct BND_Event
    {
    public:
        Type type = Type::NONE;
        BND_Event_Stats body;
    };

} // BINDU

#endif //BINDU_EVENTSTRUCT_H
