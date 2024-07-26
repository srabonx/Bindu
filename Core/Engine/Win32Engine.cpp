//
// Created by letsd on 23-Jul-24.
//

#include <windows.h>
#include "Win32Engine.h"
#include "../App/IBindu_App.h"

namespace BINDU {

    class Win32Engine::Impl
    {
    public:
        IBindu_App* m_app{nullptr};
    };

    Win32Engine::Win32Engine(IBindu_App *app) : m_impl(std::make_unique<Impl>())
    {
        m_impl->m_app = app;
    }

    void Win32Engine::Initialize()
    {
        m_impl->m_app->Initialize();
    }

    void Win32Engine::Run()
    {
        MSG msg{0};

        bool isDone = false;
        while(!isDone)
        {
            // if, have window message then process them
            if(PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
            {
                if(msg.message == WM_QUIT)
                {
                    isDone = true;
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            // else update the app logic and render
            else
            {
                m_impl->m_app->Update(0.1666);
                m_impl->m_app->Render();
            }
        }
    }

    void Win32Engine::Close()
    {
       // m_impl->m_app->Close();
    }


} // BINDU