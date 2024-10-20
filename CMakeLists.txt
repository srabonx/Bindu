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
        Core/Engine/IEngine.h
        Core/Engine/Win32Engine.cpp
        Core/Engine/Win32Engine.h
        Core/App/IBindu_App.h

        # Core - Renderer - DirectX
        Core/Renderer/DirectX/D3DDeviceManager.h
        Core/Renderer/DirectX/D3DDeviceManager.cpp
        Core/Renderer/DirectX/D3DSwapChain.h
        Core/Renderer/DirectX/D3DSwapChain.cpp
        Core/Renderer/DirectX/D3DUtillity.h
        Core/Renderer/DirectX/ComException.h
        Core/Renderer/DirectX/d3dx12.h
        Core/Renderer/DirectX/CpuDescriptorHeap.h
        Core/Renderer/DirectX/CpuDescriptorHeap.cpp
        Core/Renderer/DirectX/DescriptorHeap.h
        Core/Renderer/DirectX/DescriptorHeap.cpp
        Core/Renderer/DirectX/DescriptorHeapManager.h
        Core/Renderer/DirectX/DescriptorHeapManager.cpp
        Core/Renderer/DirectX/GpuDescriptorHeap.h 
        Core/Renderer/DirectX/GpuDescriptorHeap.cpp
        Core/Renderer/DirectX/VariableSizeGpuMemoryAllocator.h
        Core/Renderer/DirectX/VariableSizeGpuMemoryAllocator.cpp
        Core/Renderer/DirectX/VariableSizeMemoryAllocator.h
        Core/Renderer/DirectX/VariableSizeMemoryAllocator.cpp
        Core/Renderer/DirectX/D3DCommandContext.h 
        Core/Renderer/DirectX/D3DCommandContext.cpp
        Core/Renderer/DirectX/D3DFence.h
        Core/Renderer/DirectX/D3DFence.cpp
        Core/Renderer/DirectX/IDescriptorHeapAllocator.h
        Core/Renderer/DirectX/D3DPipelineStateManager.h
        Core/Renderer/DirectX/D3DPipelineStateManager.cpp
        Core/Renderer/DirectX/D3DShader.h
        Core/Renderer/DirectX/D3DShader.cpp
        Core/Renderer/DirectX/DefaultRenderContext.h
        Core/Renderer/DirectX/DefaultRenderContext.cpp

        # GameObject
        Core/GameObject/Transform.h
        Core/GameObject/Transform.cpp
        Core/GameObject/GameObject.h
        Core/GameObject/GameObject.cpp
        Core/GameObject/MeshObject.h
        Core/GameObject/MeshObject.cpp
        Core/GameObject/Buffers.h
        Core/GameObject/Buffers.cpp
        Core/GameObject/Camera.h
        Core/GameObject/Camera.cpp 
        Core/GameObject/MeshManager.h
        Core/GameObject/MeshManager.cpp
        Core/GameObject/GeometryGenerator.h
        Core/GameObject/GeometryGenerator.cpp
        Core/GameObject/Material.h
        Core/GameObject/Material.cpp
        Core/GameObject/Light.h
        Core/GameObject/Light.cpp 

        # ECS
        Core/ECS/ECSScene.h
        Core/ECS/ECSScene.cpp
        Core/ECS/EntityManager.h
        Core/ECS/EntityManager.cpp
        Core/ECS/Entity.h
        Core/ECS/Entity.cpp
        Core/ECS/Components.h
        Core/ECS/SparseSet.h
        Core/ECS/ComponentRegistry.h
        Core/ECS/View.h
        Core/ECS/ECSConstants.h

        # Managers
        Core/Managers/GameObjectManager.h
        Core/Managers/GameObjectManager.cpp
        Core/Managers/ConstantBufferManager.h
        Core/Managers/ConstantBufferManager.cpp
        Core/Managers/TextureManager.h
        Core/Managers/TextureManager.cpp
        Core/Managers/MaterialManager.h
        Core/Managers/MaterialManager.cpp
        Core/Managers/CbAllocation.h

        # Resources
        
     #   Core/Resources/Texture.cpp
        Core/Resources/DepthStencilBuffer.h
        Core/Resources/DepthStencilBuffer.cpp
        Core/Resources/RenderTexture.h
        Core/Resources/RenderTexture.cpp
        Core/Resources/Resource.h
        Core/Resources/Resource.cpp

        # Logger
        Logger/Logger.h
        Logger/Logger.cpp

        # Input
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
        "Debug/Assert.h"
        
        # APP
        Core/App/Application.h
        Core/App/Application.cpp "Core/App/AppLayer.h" "Core/App/AppLayer.cpp" "Core/App/AppLayerStack.h" "Core/App/AppLayerStack.cpp"     "Core/Renderer/Renderer.h" "Core/Renderer/GraphicsContext.h" "Core/Renderer/RendererFactory.h" "Core/Renderer/RendererFactory.cpp" "Core/Renderer/DirectX/DirectXRenderer.h" "Core/Renderer/DirectX/DirectXRenderer.cpp" "Core/Renderer/DirectX/DirectXGraphicsContext.h" "Core/Renderer/DirectX/DirectXGraphicsContext.cpp"  "Core/Renderer/RenderCommand.h" "Core/Renderer/CommandQueue.h"  "Core/Renderer/CommandList.h" "Core/Renderer/DirectX/DirectXCommandList.h" "Core/Renderer/DirectX/DirectXCommandList.cpp" "Core/Renderer/Texture.h" "Core/Renderer/DirectX/DirectXTexture.h" "Core/Renderer/DirectX/DirectXTexture.cpp" "Core/Renderer/DirectX/DirectXRenderTexture.h" "Core/Renderer/DirectX/DirectXRenderTexture.cpp" "Core/Renderer/Shader.h" "Core/Renderer/DirectX/DirectXShader.h" "Core/Renderer/DirectX/DirectXShader.cpp" "Core/Renderer/UniformBuffer.h" "Core/Renderer/DirectX/DirectXConstantBuffer.h" "Core/Renderer/DirectX/DirectXConstantBuffer.cpp" "Core/Renderer/DataFormat.h" "Core/App/EntryPoint.h")

# Link spdlog to the Bindu library
target_link_libraries(Bindu PRIVATE spdlog::spdlog)
