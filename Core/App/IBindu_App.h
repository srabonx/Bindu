//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_IBINDU_APP_H
#define BINDU_IBINDU_APP_H

namespace BINDU {

    namespace EVENT
    {
        struct BND_Event;
    }

    struct EngineStats;

    // Base class of all application
    class IBindu_App
    {
    public:
        virtual ~IBindu_App() = default;

        // Application Initialization Routine goes in here
        virtual void Initialize() = 0;

        // All the events can be delegated here through designated Event Listener.
        virtual void ProcessEvent(EVENT::BND_Event event) = 0;

        // Updates the application logic (dt = Delta Time)
        virtual void Update(double dt) = 0;

        // Renders the scene
        virtual void Render() = 0;

        // Application Closing Routine goes in here
        virtual void Close() = 0;

        // Get the engine stat
        virtual void GetEngineStat(EngineStats stats) = 0;
    };

} // BINDU

#endif //BINDU_IBINDU_APP_H
