project(
    'penrose',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20'
    ])

deps = [
    subproject('fmt').get_variable('fmt_dep'),
    subproject('glfw').get_variable('glfw_dep'),
    dependency('vulkan'),
    subproject('imgui', default_options: ['default_library=static']).get_variable('imgui_dep'),
]

src = [
    'src/Main.cpp',

    # Assets
    'src/Assets/AssetDictionary.cpp',
    'src/Assets/AssetInstance.cpp',
    'src/Assets/AssetManager.cpp',

    # Core
    'src/Core/Engine.cpp',

    # Backends
    'src/Backends/GlfwBackend.cpp',
    'src/Backends/ImGuiBackend.cpp',
    'src/Backends/VulkanBackend.cpp',

    # ECS
    'src/ECS/ECSManager.cpp',

    # ECS / Components
    'src/ECS/Components/MeshRendererComponent.cpp',

    # Events
    'src/Events/Event.cpp',
    'src/Events/EventQueue.cpp',

    # Rendering
    'src/Rendering/DeviceContext.cpp',
    'src/Rendering/PresentContext.cpp',
    'src/Rendering/RenderContext.cpp',
    'src/Rendering/RenderGraph.cpp',
    'src/Rendering/RenderGraphExecutor.cpp',
    'src/Rendering/RenderGraphParser.cpp',
    'src/Rendering/RenderThread.cpp',
    'src/Rendering/Surface.cpp',
    'src/Rendering/Utils.cpp',

    # Rendering / Operators
    'src/Rendering/Operators/ImGuiDrawRenderOperator.cpp',

    # Resources
    'src/Resources/ResourceSet.cpp',

    # Scene
    'src/Scene/Scene.cpp',
    'src/Scene/SceneManager.cpp',
]

executable('penrose', src, dependencies: deps)
