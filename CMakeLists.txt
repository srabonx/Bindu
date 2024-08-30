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
        Utility/Common/Timer.cpp 
        Gui/Common/imconfig.h
        Gui/Common/imgui.h
        Gui/Common/imgui.cpp
        Gui/Common/imgui_internal.h
        Gui/Common/imstb_rectpack.h
        Gui/Common/imstb_textedit.h
        Gui/Common/imstb_truetype.h
        Gui/Common/imgui_demo.cpp
        Gui/Common/imgui_draw.cpp
        Gui/Common/imgui_tables.cpp
        Gui/Common/imgui_widgets.cpp
        Gui/IGui.h
        Gui/DX12/imgui_impl_win32.h
        Gui/DX12/imgui_impl_win32.cpp
        Gui/DX12/imgui_impl_dx12.h
        Gui/DX12/imgui_impl_dx12.cpp
        Gui/DX12/DX12Imgui.h
        Gui/DX12/DX12Imgui.cpp 
        Core/Renderer/DirectX/RenderTexture.h
        Core/Renderer/DirectX/RenderTexture.cpp
        Core/Renderer/DirectX/Utility/DescriptorHeap.h
        Core/Renderer/DirectX/Utility/DescriptorHeap.cpp 
        Core/Renderer/DirectX/Utility/UploadBuffer.h 
        Core/Renderer/DirectX/Utility/MeshGeometry.h
        Utility/Common/CircleBuffer.h
        Core/Renderer/DirectX/Utility/GPUDescriptorHeapManager.h
        Core/Renderer/DirectX/Utility/GPUDescriptorHeapManager.cpp
        Core/Renderer/DirectX/Utility/VariableSizeMemoryAllocator.h
        Core/Renderer/DirectX/Utility/VariableSizeMemoryAllocator.cpp
        Core/Renderer/DirectX/Utility/VariableSizeGpuMemoryAllocator.h
        Core/Renderer/DirectX/Utility/VariableSizeGpuMemoryAllocator.cpp
         "Core/Renderer/DirectX/Utility/DescriptorHeapManager.h" "Core/Renderer/DirectX/Utility/DescriptorHeapManager.cpp")


