//
// Created by letsd on 23-Jul-24.
//

#include <windowsx.h>

#include "Win32Window.h"

#include "../Event/EventManager.h"
#include "../Event/EventStruct.h"
#include "../Utility/Common/CommonUtility.h"
#include "../Input/Keyboard.h"

namespace BINDU {

    class Win32Window::Impl
    {
    public:
        //Application Instance handle
        HINSTANCE m_hInstance{nullptr};

        //Ev_Window Handle
        HWND m_hWnd{nullptr};

        //Ev_Window Width and Height
        uint16_t m_width{200};
        uint16_t m_height{200};

        //Ev_Window title
        std::string m_title;

      /*  // pointer to the Event Manager
        EventManager* m_eventManager{nullptr}; */

    public:
        static LRESULT CALLBACK WindowMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT MessageProc(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam);
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
         THROW_EXCEPTION(1, "Error registering Ev_Window class");
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

    void Win32Window::Update()
    {
        MSG msg{};

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    void Win32Window::RegisterEventManager(EventManager *eventManager)
    {
       // m_impl->m_eventManager = eventManager;
    }

    void Win32Window::SetWindowTitle(const std::string& title)
    {
        m_impl->m_title = title;
        SetWindowTextA(m_impl->m_hWnd, title.c_str());
    }

    std::string Win32Window::GetWindowTitle() const
    {
        return m_impl->m_title;
    }

    UINT Win32Window::GetHeight() const
    {
        return m_impl->m_height;
    }

    UINT Win32Window::GetWidth() const
    {
        return m_impl->m_width;
    }

    HWND& Win32Window::GetHandle() {
        return m_impl->m_hWnd;
    }


    LRESULT Win32Window::Impl::WindowMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));

        if (window)
        {
            window->m_impl->MessageProc( hWnd, msg, wParam, lParam);
        }

        

        return DefWindowProcA(hWnd,msg,wParam,lParam);
    }

    LRESULT Win32Window::Impl::MessageProc(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam)
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

                event.Ev_Window.Width = m_width;
                event.Ev_Window.Height = m_height;

                break;
            case WM_EXITSIZEMOVE:
                event.type = EVENT::Type::WINDOW_EXIT_RESIZING;

                event.Ev_Window.Width = m_width;
                event.Ev_Window.Height = m_height;

                break;
            case WM_SIZE:
            {
                event.type = EVENT::Type::WINDOW_SIZE_CHANGED;
                event.Ev_Window.Width = LOWORD(lParam);
                event.Ev_Window.Height = HIWORD(lParam);

                m_width = LOWORD(lParam);
                m_height = HIWORD(lParam);

                // Push the WINDOW_SIZE_CHANGED Event
               // if (m_eventManager)
                 //   m_eventManager->PushEvent(event);
                EventManager::PushEvent(event);

                if (wParam == SIZE_MAXIMIZED)
                    event.type = EVENT::Type::WINDOW_MAXIMIZED;
                else if (wParam == SIZE_MINIMIZED)
                    event.type = EVENT::Type::WINDOW_MINIMIZED;
                else if (wParam == SIZE_RESTORED)
                    event.type = EVENT::Type::WINDOW_RESTORED;

                break;
            }
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN: {
                event.type = EVENT::Type::KEY_DOWN;
                WORD vkCode = LOWORD(wParam);

                event.Ev_Keyboard.key = static_cast<BND_Key>(vkCode);
                event.Ev_Keyboard.pressed = true;
                break;
            }
            case WM_SYSKEYUP:
            case WM_KEYUP: {
                event.type = EVENT::Type::KEY_UP;
                WORD vkCode = LOWORD(wParam);

                event.Ev_Keyboard.key = static_cast<BND_Key>(vkCode);
                event.Ev_Keyboard.pressed = false;

                break;
            }

	        case WM_LBUTTONDOWN:
	        case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:

                SetCapture(m_hWnd);

                event.type = EVENT::Type::MOUSE_DOWN;

                event.Ev_Mouse.button = BND_Button::BND_NONE;

                if (wParam & MK_LBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_LEFT;
                else if(wParam & MK_RBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_RIGHT;
                else if(wParam & MK_MBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_MIDDLE;


                event.Ev_Mouse.position.x = GET_X_LPARAM(lParam);
                event.Ev_Mouse.position.y = GET_Y_LPARAM(lParam);

                break;

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:

                ReleaseCapture();
				
                event.type = EVENT::Type::MOUSE_UP;

                event.Ev_Mouse.button = BND_Button::BND_NONE;

                if (msg == WM_LBUTTONUP)
                    event.Ev_Mouse.button = BND_Button::BND_LEFT;
                else if (msg == WM_RBUTTONUP)
                    event.Ev_Mouse.button = BND_Button::BND_RIGHT;
                else if (msg == WM_MBUTTONUP)
                    event.Ev_Mouse.button = BND_Button::BND_MIDDLE;


                event.Ev_Mouse.position.x = GET_X_LPARAM(lParam);
                event.Ev_Mouse.position.y = GET_Y_LPARAM(lParam);

                break;

			case WM_MOUSEMOVE:

                event.type = EVENT::Type::MOUSE_MOVE;

                event.Ev_Mouse.button = BND_Button::BND_NONE;

                if (wParam & MK_LBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_LEFT;
                else if (wParam & MK_RBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_RIGHT;
                else if (wParam & MK_MBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_MIDDLE;


                event.Ev_Mouse.position.x = GET_X_LPARAM(lParam);
                event.Ev_Mouse.position.y = GET_Y_LPARAM(lParam);

                break;

            case WM_MOUSEWHEEL:
                event.type = EVENT::Type::MOUSE_SCROLL;

                event.Ev_Mouse.button = BND_Button::BND_NONE;

                if (wParam & MK_LBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_LEFT;
                else if (wParam & MK_RBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_RIGHT;
                else if (wParam & MK_MBUTTON)
                    event.Ev_Mouse.button = BND_Button::BND_MIDDLE;


                event.Ev_Mouse.position.x = GET_X_LPARAM(lParam);
                event.Ev_Mouse.position.y = GET_Y_LPARAM(lParam);

                event.Ev_Mouse.mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

                break;
            default:
                event.type = EVENT::Type::NONE;
                break;
        }

       // if(m_eventManager)
         //   m_eventManager->PushEvent(event);
        EventManager::PushEvent(event);

        return LRESULT();
    }


} // BINDU