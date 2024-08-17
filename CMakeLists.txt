cmake_minimum_required(VERSION 3.27.2)
project(Bindu)

set(CMAKE_CXX_STANDARD 17)


add_library(Bindu STATIC
        ErrorHandler/EngineException.h
        ErrorHandler/EngineException.cpp
        Event/EventManager.cpp
        Event/EventManager.h
        Event/EventStruct.cpp
        Event/EventStruct.h
        Event/EventPool.cpp
        Event/EventPool.h
        Event/IEventListener.h
        Window/IWindow.h
        Window/Win32Window.cpp
        Window/Win32Window.h
        Core/Engine/IEngine.h
        Core/Engine/Win32Engine.cpp
        Core/Engine/Win32Engine.h
        Core/App/IBindu_App.h
        Utility/Common/CommonUtility.h
        Logger/ILogger.h
        Logger/ErrorLogManager.cpp
        Logger/ErrorLogManager.h
        Logger/Logger.h
        Logger/Logger.cpp
        Input/InputManager.cpp
        Input/InputManager.h
        Input/KeysAndButtons.h
        Input/IInputHandler.h
        Input/Win32InputHandler.cpp
        Input/Win32InputHandler.h
        Input/Keyboard.cpp
        Input/Keyboard.h
        Input/Mouse.h
        Input/Mouse.cpp
        Core/Renderer/IRenderer.h
        Core/Renderer/DirectX/DX12Renderer.cpp
        Core/Renderer/DirectX/DX12Renderer.h
        Core/Renderer/DirectX/Utility/D3DUtillity.h
        Core/Renderer/DirectX/Error/ComException.h
        Core/Renderer/DirectX/Utility/d3dx12.h
        Utility/Common/Timer.h
        Utility/Common/Timer.cpp)


