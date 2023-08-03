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
    # TODO: https://github.com/nothings/stb
]

incdir = include_directories('include')

src = [
    # Assets
    'src/Assets/AssetDictionary.cpp',
    'src/Assets/AssetReader.cpp',
    'src/Assets/AssetManager.cpp',

    # Common
    'src/Common/ParamsCollection.cpp',

    # Core
    'src/Core/Engine.cpp',

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
    'src/Rendering/RenderGraphExecutorProvider.cpp',
    'src/Rendering/RenderListBuilder.cpp',
    'src/Rendering/RenderManager.cpp',
    'src/Rendering/Surface.cpp',
    'src/Rendering/Utils.cpp',

    # Resources
    'src/Resources/ResourceSet.cpp',

    # Scene
    'src/Scene/SceneManager.cpp',
    'src/Scene/SceneNode.cpp',

    # Builtin / Assets
    'src/Builtin/Assets/VkImageAsset.cpp',
    'src/Builtin/Assets/VkMeshAsset.cpp',
    'src/Builtin/Assets/VkShaderAsset.cpp',

    # Builtin / Backends
    'src/Builtin/Backends/GlfwBackend.cpp',
    'src/Builtin/Backends/ImGuiBackend.cpp',
    'src/Builtin/Backends/VulkanBackend.cpp',

    # Builtin / Rendering
    'src/Builtin/Rendering/ForwardSceneDrawRenderOperator.cpp',
    'src/Builtin/Rendering/ImGuiDrawRenderOperator.cpp',
    'src/Builtin/Rendering/VkFramebuffer.cpp',
    'src/Builtin/Rendering/VkRenderPass.cpp',
    'src/Builtin/Rendering/VkRenderTarget.cpp',
    'src/Builtin/Rendering/VkUtils.cpp',
]

cpp_args = [
    '-DSTB_IMAGE_IMPLEMENTATION',
    '-DTINYOBJLOADER_IMPLEMENTATION',
    '-DGLM_FORCE_DEPTH_ZERO_TO_ONE'
]

penrose = library('penrose', src, dependencies: deps, cpp_args: cpp_args, include_directories: incdir)
penrose_dep = declare_dependency(include_directories: incdir, link_with: penrose)
