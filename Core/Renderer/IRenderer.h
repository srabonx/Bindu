//
// Created by letsd on 13-Aug-24.
//

#ifndef BINDU_IRENDERER_H
#define BINDU_IRENDERER_H

#include "../../Window/IWindow.h"

namespace BINDU {

    class IRenderer
    {
    public:
        
        virtual ~IRenderer() {}

        // Initialize the renderer
        virtual void Initialize(IWindow* window) = 0;

        // Clear the screen with the specified color
        virtual void ClearScreen(float r, float g, float b, float a) = 0;

        // Present the render to the display
        virtual void Present() = 0;

        // Set the renderer to the passed Window
        virtual void SetTo(IWindow* window) = 0;

        // Close the renderer
        virtual void Close() = 0;


    };

} // BINDU

#endif //BINDU_IRENDERER_H
