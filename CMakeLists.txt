cmake_minimum_required(VERSION 3.27.2)
project(Bindu)

set(CMAKE_CXX_STANDARD 17)

# FetchContent to get spdlog from GitHub
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.11.0 # Specify the version you want
)
FetchContent_MakeAvailable(spdlog)

# Define the Bindu library
add_library(Bindu STATIC
        # ErrorHandler
        ErrorHandler/EngineException.h
        ErrorHandler/EngineException.cpp

        # Event
        Event/EventManager.cpp
        Event/EventManager.h
        Event/EventStruct.cpp
        Event/EventStruct.h
        Event/EventPool.cpp
        Event/EventPool.h
        Event/IEventListener.h

        # Window
        Window/IWindow.h
        Window/Win32Window.cpp
        Window/Win32Window.h

        # Core - Engine
        Core/App/IBindu_App.h

        #Renderer - DirectX
        Renderer/DirectX/D3DDeviceManager.h
        Renderer/DirectX/D3DDeviceManager.cpp
        Renderer/DirectX/D3DUtillity.h
        Renderer/DirectX/ComException.h
        Renderer/DirectX/d3dx12.h
        Renderer/DirectX/CpuDescriptorHeap.h
        Renderer/DirectX/CpuDescriptorHeap.cpp
        Renderer/DirectX/DescriptorHeap.h
        Renderer/DirectX/DescriptorHeap.cpp
        Renderer/DirectX/DescriptorHeapManager.h
        Renderer/DirectX/DescriptorHeapManager.cpp
        Renderer/DirectX/GpuDescriptorHeap.h 
        Renderer/DirectX/GpuDescriptorHeap.cpp
        Renderer/DirectX/VariableSizeGpuMemoryAllocator.h
        Renderer/DirectX/VariableSizeGpuMemoryAllocator.cpp
        Renderer/DirectX/VariableSizeMemoryAllocator.h
        Renderer/DirectX/VariableSizeMemoryAllocator.cpp
        Renderer/DirectX/D3DCommandContext.h 
        Renderer/DirectX/D3DCommandContext.cpp
        Renderer/DirectX/DDSTextureLoader.h 
        Renderer/DirectX/DDSTextureLoader.cpp
        Renderer/DirectX/D3DFence.h
        Renderer/DirectX/D3DFence.cpp
        Renderer/DirectX/IDescriptorHeapAllocator.h
        Renderer/DirectX/D3DPipelineStateManager.h
        Renderer/DirectX/D3DPipelineStateManager.cpp
        Renderer/DirectX/D3DShader.h
        Renderer/DirectX/D3DShader.cpp

        # ECS
        Scene/Scene.h
        Scene/Scene.cpp
        ECS/EntityManager.h
        ECS/EntityManager.cpp
        ECS/Entity.h
        ECS/Entity.cpp
        ECS/Components.h
        ECS/SparseSet.h
        ECS/ComponentRegistry.h
        ECS/View.h
        ECS/ECSConstants.h

        # Logger
        Logger/Logger.h
        Logger/Logger.cpp

        # Input
        Input/InputManager.cpp
        Input/InputManager.h
        
        Input/IInputHandler.h
        Input/Win32InputHandler.cpp
        Input/Win32InputHandler.h
        Input/Keyboard.cpp
        Input/Keyboard.h
        Input/Mouse.h
        Input/Mouse.cpp

        # Utility - Common
        Utility/Common/CommonUtility.h
        Utility/Common/CircleBuffer.h 
        Utility/Common/Timer.h
        Utility/Common/Timer.cpp
        Utility/Common/DynamicVoidBuffer.h
        Utility/Common/DynamicVoidBuffer.cpp
        Utility/Common/DynamicBitset.h
        Utility/Common/DynamicBitset.cpp
        MathHelper/MathHelper.h
        MathHelper/MathHelper.cpp
        
        # DEBUG
        Debug/Profiler.h
        Debug/Profiler.cpp
        Debug/Assert.h
        
        # APP
        Core/App/Application.h
        Core/App/Application.cpp "Core/App/AppLayer.h" "Core/App/AppLayer.cpp" "Core/App/AppLayerStack.h" "Core/App/AppLayerStack.cpp"     "Renderer/Renderer.h" "Renderer/GraphicsContext.h" "Renderer/RendererFactory.h" "Renderer/RendererFactory.cpp" "Renderer/DirectX/DirectXRenderer.h" "Renderer/DirectX/DirectXRenderer.cpp" "Renderer/DirectX/DirectXGraphicsContext.h" "Renderer/DirectX/DirectXGraphicsContext.cpp"  "Renderer/RenderCommand.h" "Renderer/CommandQueue.h"  "Renderer/CommandList.h" "Renderer/DirectX/DirectXCommandList.h" "Renderer/DirectX/DirectXCommandList.cpp" "Renderer/Texture.h" "Renderer/DirectX/DirectXTexture.h" "Renderer/DirectX/DirectXTexture.cpp" "Renderer/DirectX/DirectXRenderTexture.h" "Renderer/DirectX/DirectXRenderTexture.cpp" "Renderer/Shader.h" "Renderer/DirectX/DirectXGraphicsShader.h" "Renderer/DirectX/DirectXGraphicsShader.cpp" "Renderer/UniformBuffer.h" "Renderer/DirectX/DirectXConstantBuffer.h" "Renderer/DirectX/DirectXConstantBuffer.cpp" "Renderer/DataFormat.h" "Renderer/ResourceFactory.h" "Renderer/ResourceFactory.cpp" "Renderer/Buffer.h" "Renderer/DirectX/DirectXBuffer.h" "Renderer/DirectX/DirectXBuffer.cpp" "Geometry/MeshData.h" "Scene/SceneLayer.h" "Camera/Camera.h" "Camera/EditorCamera.h" "Camera/EditorCamera.cpp" "Camera/OrthographicCamera.h" "Camera/OrthographicCamera.cpp"    "Camera/PerspectiveCamera.h" "Camera/PerspectiveCamera.cpp"   "Camera/CameraController.h" "Camera/CameraController.cpp" "Camera/SceneCameraController.h" "Camera/SceneCameraController.cpp" "Camera/ICameraController.h" "Geometry/GeometryGenerator.cpp" "Geometry/GeometryGenerator.h" "Geometry/MeshManager.cpp" "Geometry/MeshManager.h" "Renderer/ShaderLibrary.h" "Renderer/ShaderLibrary.cpp")

# Link spdlog to the Bindu library
target_link_libraries(Bindu PRIVATE spdlog::spdlog_header_only)
target_compile_definitions(Bindu PRIVATE SPDLOG_HEADER_ONLY)

