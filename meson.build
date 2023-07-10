project(
    'penrose',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20'
    ])

cmake = import('cmake')
cmake_opts = cmake.subproject_options()
cmake_opts.add_cmake_defines({'CMAKE_POSITION_INDEPENDENT_CODE': true})

deps = [
    subproject('fmt').get_variable('fmt_dep'),
    subproject('glfw').get_variable('glfw_dep'),
    dependency('vulkan'),
    subproject('imgui', default_options: ['default_library=static']).get_variable('imgui_dep'),
    subproject('glm').get_variable('glm_dep'),
    cmake.subproject('tinyobjloader', options: cmake_opts).dependency('tinyobjloader'),
]

incdir = include_directories('include')

src = [
    # Assets
    'src/Assets/AssetDictionary.cpp',
    'src/Assets/AssetInstance.cpp',
    'src/Assets/AssetManager.cpp',

    # Common
    'src/Common/ParamsCollection.cpp',

    # Core
    'src/Core/Engine.cpp',

    # Backends
    'src/Backends/GlfwBackend.cpp',
    'src/Backends/ImGuiBackend.cpp',
    'src/Backends/VulkanBackend.cpp',

    # ECS
    'src/ECS/ECSManager.cpp',

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

    # Resources
    'src/Resources/ResourceSet.cpp',

    # Scene
    'src/Scene/Scene.cpp',
    'src/Scene/SceneManager.cpp',

    # Builtin

    # ECS / Systems
    'src/Builtin/ECS/RenderListBuilderSystem.cpp',

    # Rendering / Operators
    'src/Builtin/Rendering/ForwardSceneDrawRenderOperator.cpp',
    'src/Builtin/Rendering/ImGuiDrawRenderOperator.cpp',
]

cpp_args = [
    '-DTINYOBJLOADER_IMPLEMENTATION',
    '-DGLM_FORCE_DEPTH_ZERO_TO_ONE'
]

penrose = library('penrose', src, dependencies: deps, cpp_args: cpp_args, include_directories: incdir)
penrose_dep = declare_dependency(include_directories: incdir, link_with: penrose)
