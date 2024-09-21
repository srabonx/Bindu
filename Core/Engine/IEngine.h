//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_IENGINE_H
#define BINDU_IENGINE_H

namespace BINDU {

    using uint16_t = unsigned short;
    using uint32_t = unsigned int;
    using uint64_t = unsigned long long;

    struct EngineCFG
    {
        // Target Frames per second the engine should try to render
        uint16_t    TargetFPS = 60;
        // Target Ticks per second the engine should try to update
        uint16_t    TargetTPS = 60;

    };

    struct EngineStats
    {
	    // The values of EngineStats will be updates each second

        // Total time in seconds since initialization
        uint64_t    UpTime = 0;

        // Frames per second count
        uint32_t    FPS = 0;

        // Ticks per second count
        uint32_t    TPS = 0;

    };

    class IEngine
    {
    public:

        virtual ~IEngine() = default;

        // Initialize Routine goes in here
        virtual void Initialize(const EngineCFG& engineCfg) = 0;

        // Run the Main Loop
        virtual void Run() = 0;

        virtual void RunUnlocked() = 0;

        // Closing Routine goes in here
        virtual void Close() = 0;

    };

} // BINDU

#endif //BINDU_IENGINE_H
