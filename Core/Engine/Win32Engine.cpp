//
// Created by letsd on 23-Jul-24.
//
#define NOMINMAX
#include <windows.h>
#include "Win32Engine.h"

#include <sstream>
#include <thread>

#include "../App/IBindu_App.h"
#include "../../Logger/Logger.h"

namespace BINDU {

    class Win32Engine::Impl
    {
    public:
        IBindu_App* m_app{nullptr};

        EngineCFG   m_engineCfg;

        EngineStats m_engineStats;
    };

    Win32Engine::Win32Engine(IBindu_App *app) : m_impl(std::make_unique<Impl>())
    {
        // Open the log file
        if (!Logger::Exists())
            Logger::Get()->Open("EngineLog.log");

        m_impl->m_app = app;
    }

    void Win32Engine::Initialize(const EngineCFG& engineCfg)
    {
        m_impl->m_engineCfg = engineCfg;
        m_impl->m_app->Initialize();
    }

    //void Win32Engine::Run()
    //{


    //    using frame = std::chrono::duration<int32_t, std::ratio<1, 60>>;
    //    using ms = std::chrono::duration<float, std::milli>;

    //    std::chrono::time_point<std::chrono::steady_clock> fpsTimer(std::chrono::steady_clock::now());
    //    frame FPS{};

    //    MSG msg{0};

    //    bool isDone = false;
    //    while(!isDone)
    //    {
    //        FPS = std::chrono::duration_cast<frame>(std::chrono::steady_clock::now() - fpsTimer);

    //        if (FPS.count() >= 1)
    //        {
    //            fpsTimer = std::chrono::steady_clock::now();

    //            double deltaTime = std::chrono::duration_cast<ms>(FPS).count();

    //            // if, have window message then process them
    //            if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
    //            {
    //                if (msg.message == WM_QUIT)
    //                {
    //                    isDone = true;
    //                    break;
    //                }

    //                TranslateMessage(&msg);
    //                DispatchMessageA(&msg);
    //            }
    //            // else update the app logic and render
    //            else
    //            {
    //                m_impl->m_app->Update(deltaTime);
    //                m_impl->m_app->Render();
    //            }
    //        }
    //        else
    //            Sleep(1);
    //    }
    //}


    void Win32Engine::Run()
    {

        long tickAccumulator = 0;
        long frameAccumulator = 0;

        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        TimePoint endTime = std::chrono::steady_clock::now();
        TimePoint startTime = std::chrono::steady_clock::now();


        std::chrono::steady_clock::duration elapsedTime = {};

        long frameCount = 0;
        long tickCount = 0;
        long fpsElapsedTime = 0;

        const uint16_t fps = m_impl->m_engineCfg.TargetFPS;
        const  long lengthOfFrame = static_cast<long>(std::chrono::steady_clock::period::den) / fps;
        const uint16_t tps = m_impl->m_engineCfg.TargetTPS;
        const long lengthOfTick = static_cast<long>(std::chrono::steady_clock::period::den) / tps;

        long longestTime = std::max(lengthOfFrame, lengthOfTick);


        // Tick delta in seconds
        const double tickDelta = 1.0 / tps;


        MSG msg{ 0 };

        bool isDone = false;
        while (!isDone)
        {
            startTime = std::chrono::steady_clock::now();

            elapsedTime = startTime - endTime;

            endTime = startTime;

            long delta = static_cast<long>(elapsedTime.count());

            tickAccumulator += delta;

            if (elapsedTime.count() == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(5));

            while(PeekMessageA(&msg, nullptr, 0,0,PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    isDone = true;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            if (isDone)
                break;

            // Update loop is independent of rendering
            while(tickAccumulator >= lengthOfTick)
            {
            	// Update
                m_impl->m_app->Update(tickDelta);

                // finish tick
                tickAccumulator -= lengthOfTick;

                // keep track of how many tick we've done this second
                tickCount++;
            }

            frameAccumulator += delta;

            while (frameAccumulator >= lengthOfFrame)
            {

                // Render the frame
                m_impl->m_app->Render();

                frameAccumulator -= lengthOfFrame;

                // keep track of how many frame we've done this second
                frameCount++;
            }

            // Update the elapsed time
            fpsElapsedTime += delta;

            if(fpsElapsedTime >= std::chrono::steady_clock::period::den)
            {

                m_impl->m_engineStats.UpTime++;
                m_impl->m_engineStats.FPS = frameCount;
                m_impl->m_engineStats.TPS = tickCount;

                m_impl->m_app->GetEngineStat(m_impl->m_engineStats);

                fpsElapsedTime = 0;
                frameCount = 0;
                tickCount = 0;
            }

            long timeToSleep = longestTime - delta;

            if (timeToSleep > 0 && timeToSleep < longestTime)
                std::this_thread::sleep_for(std::chrono::nanoseconds(timeToSleep));

        }

    }

    void Win32Engine::RunUnlocked()
    {

        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        TimePoint endTime = std::chrono::steady_clock::now();
        TimePoint startTime = std::chrono::steady_clock::now();


        std::chrono::steady_clock::duration elapsedTime = {};


        MSG msg{ 0 };

        bool isDone = false;
        while (!isDone)
        {

            startTime = std::chrono::steady_clock::now();

            elapsedTime = startTime - endTime;

            endTime = startTime;

            long delta = static_cast<long>(elapsedTime.count());

            double deltaSec = delta * 0.000000001;

            while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    isDone = true;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            if (isDone)
                break;

            m_impl->m_app->Update(deltaSec);
            m_impl->m_app->Render();

        }

    }


    void Win32Engine::Close()
    {
        Logger::Get()->Close();
       // m_impl->m_app->Close();
    }

} // BINDU