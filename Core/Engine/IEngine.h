//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_IENGINE_H
#define BINDU_IENGINE_H

namespace BINDU {

    class IEngine
    {
    public:

        virtual ~IEngine() = default;

        // Initialize Routine goes in here
        virtual void Initialize() = 0;

        // Run the Main Loop
        virtual void Run() = 0;

        // Closing Routine goes in here
        virtual void Close() = 0;
    };

} // BINDU

#endif //BINDU_IENGINE_H
