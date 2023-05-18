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
]

src = [
    'src/main.cpp',

    # Core
    'src/Core/Engine.cpp',
    'src/Core/ResourceSet.cpp',

    # Backends
    'src/Backends/GlfwBackend.cpp',
    'src/Backends/VulkanBackend.cpp',

    # Events
    'src/Events/EventQueue.cpp',

    # Rendering
    'src/Rendering/DeviceContext.cpp',
    'src/Rendering/PresentContext.cpp',
    'src/Rendering/RenderThread.cpp',
    'src/Rendering/Surface.cpp',
]

executable('penrose', src, dependencies: deps)
