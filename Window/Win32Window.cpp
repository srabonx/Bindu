//
// Created by letsd on 23-Jul-24.
//

#include "Win32Window.h"
#include "../Event/EventManager.h"
#include "../Event/EventStruct.h"
#include "../Utility/Common/CommonUtility.h"

namespace BINDU {

    class Win32Window::Impl
    {
    public:
        //Application Instance handle
        HINSTANCE m_hInstance{nullptr};

        //Window Handle
        HWND m_hWnd{nullptr};

        //Window Width and Height
        uint16_t m_width{200};
        uint16_t m_height{200};

        //Window title
        std::string m_title;

        // pointer to the Event Manager
        EventManager* m_eventManager{nullptr};

    public:
        static LRESULT CALLBACK WindowMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT MessageProc(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam) const;
    };

    Win32Window::Win32Window(HINSTANCE hInstance) : m_impl(std::make_unique<Impl>())
    {
        m_impl->m_hInstance = hInstance;
    }

    // Creating the window.
    void Win32Window::Create(uint16_t width, uint16_t height, std::string title)
    {
        // Cache the width, height and title of the window
        m_impl->m_width = width;
        m_impl->m_height = height;
        m_impl->m_title = std::move(title);

     WNDCLASSEXA wndclass;
     ZeroMemory(&wndclass,sizeof(WNDCLASSEXA));

     wndclass.cbClsExtra = 0;
     wndclass.cbSize = sizeof(WNDCLASSEXA);
     wndclass.cbWndExtra = 0;
     wndclass.hCursor = LoadCursorA(m_impl->m_hInstance,IDC_ARROW);
     wndclass.hIcon = LoadIconA(m_impl->m_hInstance,IDI_APPLICATION);
     wndclass.hIconSm = nullptr;
     wndclass.hInstance = m_impl->m_hInstance;
     wndclass.hbrBackground = nullptr;
     wndclass.lpfnWndProc = m_impl->WindowMessageProc;
     wndclass.lpszClassName = "MAINWINDOW";
     wndclass.lpszMenuName = nullptr;
     wndclass.style = CS_HREDRAW | CS_VREDRAW;

     if(!RegisterClassEx(&wndclass))
     {
         THROW_EXCEPTION(1, "Error registering Window class");
     }

     m_impl->m_hWnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "MAINWINDOW",
                                      m_impl->m_title.c_str(),
                                      WS_OVERLAPPEDWINDOW,
                                      CW_USEDEFAULT, CW_USEDEFAULT,

                                      m_impl->m_width,m_impl->m_height,
                                      nullptr,
                                      nullptr,
                                      m_impl->m_hInstance,
                                      nullptr );

     if(m_impl->m_hWnd == nullptr)
     {
         THROW_EXCEPTION(2, "Error creating window handle")
     }

        ShowWindow(m_impl->m_hWnd, SW_SHOW);
        UpdateWindow(m_impl->m_hWnd);

        SetWindowLongPtr(m_impl->m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    void Win32Window::RegisterEventManager(EventManager *eventManager)
    {
        m_impl->m_eventManager = eventManager;
    }


    LRESULT Win32Window::Impl::WindowMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));

        if(window)
            window->m_impl->MessageProc(hWnd,msg,wParam,lParam);

        return DefWindowProcA(hWnd,msg,wParam,lParam);
    }

    LRESULT Win32Window::Impl::MessageProc(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam) const
    {

        EVENT::BND_Event event;
        switch(msg)
        {
            case WM_QUIT:
                event.type = EVENT::Type::QUIT;
                break;
            case WM_DESTROY:
                event.type = EVENT::Type::DESTROY;
                break;
            case WM_ACTIVATE:

                if(LOWORD(wParam) == WA_INACTIVE)
                    event.type = EVENT::Type::WINDOW_INACTIVE;
                else
                    event.type = EVENT::Type::WINDOW_ACTIVE;

                break;
            case WM_ENTERSIZEMOVE:
                event.type = EVENT::Type::WINDOW_ENTER_RESIZING;
                break;
            case WM_EXITSIZEMOVE:
                event.type = EVENT::Type::WINDOW_EXIT_RESIZING;
                break;
            case WM_SIZE:
                event.type = EVENT::Type::WINDOW_SIZE_CHANGED;
                event.body.Window.Width = LOWORD(lParam);
                event.body.Window.Height = HIWORD(lParam);

                if(wParam == SIZE_MAXIMIZED)
                    event.type = EVENT::Type::WINDOW_MAXIMIZED;
                else if(wParam == SIZE_MINIMIZED)
                    event.type = EVENT::Type::WINDOW_MINIMIZED;
                else if(wParam == SIZE_RESTORED)
                    event.type = EVENT::Type::WINDOW_RESTORED;

                break;

            case WM_KEYDOWN:
                event.type = EVENT::Type::KEY_DOWN;
                break;
            case WM_KEYUP:
                event.type = EVENT::Type::KEY_UP;
                break;

            default:
                event.type = EVENT::Type::NONE;
                break;
        }

        if(m_eventManager)
            m_eventManager->PushEvent(event);


        return LRESULT();
    }


} // BINDU