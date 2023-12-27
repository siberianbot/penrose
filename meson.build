project(
    'penrose',
    'cpp',
    version : '0.1',
    default_options : [
        'warning_level=3',
        'cpp_std=c++20',
        'default_library=static'
    ])

deps = [
    dependency('fmt'),
    dependency('glfw3'),
    dependency('glm'),
    dependency('imgui'),
    dependency('libxml-2.0'),
    dependency('libxml++'),
    dependency('nlohmann_json'),
    dependency('vulkan'),
]

incdir = include_directories('include')

src = [
    # Core
    'src/Engine.cpp',

    # Assets
    'src/Assets/AssetIndex.cpp',
    'src/Assets/AssetLoadingJobQueue.cpp',
    'src/Assets/AssetLoadingProxy.cpp',
    'src/Assets/AssetManagerImpl.cpp',
    'src/Assets/AssetReader.cpp',
    'src/Assets/Utils.cpp',

    # Assets / Loaders
    'src/Assets/Loaders/ImageLoader.cpp',
    'src/Assets/Loaders/MeshLoader.cpp',
    'src/Assets/Loaders/ShaderLoader.cpp',
    'src/Assets/Loaders/UILayoutLoader.cpp',

    # Common
    'src/Common/ConsoleLogSink.cpp',
    'src/Common/JobQueue.cpp',
    'src/Common/LogImpl.cpp',

    # ECS
    'src/ECS/ComponentFilterIterator.cpp',
    'src/ECS/EntityFilterIterator.cpp',
    'src/ECS/EntityManagerImpl.cpp',
    'src/ECS/EntityQuery.cpp',
    'src/ECS/EntityStore.cpp',
    'src/ECS/SystemManager.cpp',

    # Input
    'src/Input/Input.cpp',
    'src/Input/InputHandler.cpp',

    # Performance
    'src/Performance/Profiler.cpp',

    # Rendering
    'src/Rendering/DefaultDrawableProvider.cpp',
    'src/Rendering/DefaultRenderer.cpp',
    'src/Rendering/DefaultViewProvider.cpp',
    'src/Rendering/RenderListBuilder.cpp',
    'src/Rendering/RenderManagerImpl.cpp',
    'src/Rendering/SurfaceManager.cpp',

    # Resources
    'src/Resources/ResourceSet.cpp',

    # Scene
    'src/Scene/SceneManager.cpp',
    'src/Scene/SceneNode.cpp',

    # UI
    'src/UI/Layout.cpp',
    'src/UI/LayoutFactory.cpp',
    'src/UI/UIManager.cpp',
    'src/UI/Value.cpp',
    'src/UI/Property.cpp',

    # Builtin / Inspector
    'src/Builtin/Inspector/Inspector.cpp',

    # Builtin / GLFW
    'src/Builtin/Glfw/Glfw.cpp',
    'src/Builtin/Glfw/GlfwBackend.cpp',
    'src/Builtin/Glfw/Rendering/GlfwSurface.cpp',
    'src/Builtin/Glfw/Rendering/GlfwSurfaceController.cpp',
    'src/Builtin/Glfw/Utils/InputUtils.cpp',

    # Builtin / ImGui
    'src/Builtin/ImGui/ImGui.cpp',
    'src/Builtin/ImGui/ImGuiBackend.cpp',
    'src/Builtin/ImGui/Rendering/VkImGuiRenderer.cpp',
    'src/Builtin/ImGui/UI/ImGuiUIContextVisitor.cpp',

    # Builtin / Vulkan
    'src/Builtin/Vulkan/Vulkan.cpp',

    'src/Builtin/Vulkan/VulkanBackend.cpp',
    'src/Builtin/Vulkan/Rendering/VkCommandRecorder.cpp',
    'src/Builtin/Vulkan/Rendering/VkMemoryAllocator.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderContext.cpp',
    'src/Builtin/Vulkan/Rendering/VkRendererContext.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSystem.cpp',
    'src/Builtin/Vulkan/Rendering/VkSurfacePreferencesProvider.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkBufferFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkImageFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkInternalObjectFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkPipelineFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceSelector.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkSamplerFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkShaderFactory.cpp',
    'src/Builtin/Vulkan/Rendering/Objects/VkSwapchainFactory.cpp',
]

base_cpp_args = [
    '-DGLM_FORCE_DEPTH_ZERO_TO_ONE'
]

#    penrose_api_value = '__attribute__ ((visibility ("default")))'

#    if host_machine.system() == 'windows'
#        penrose_api_value = get_option('default_library') != 'static' ? '__declspec(dllexport)' : ''
#    endif

lib_cpp_args = base_cpp_args + [
    '-DPENROSE_API='
]

#    if host_machine.system() == 'windows'
#        penrose_api_value = get_option('default_library') != 'static' ? '__declspec(dllimport)' : ''
#    endif

dep_cpp_args = base_cpp_args + [
    '-DPENROSE_API='
]

penrose_lib = library(
    'penrose',
    src,
    dependencies : deps,
    cpp_args : lib_cpp_args,
    include_directories : incdir,
    gnu_symbol_visibility : 'hidden'
)

penrose_dep = declare_dependency(
    dependencies : deps,
    compile_args : dep_cpp_args,
    include_directories : incdir,
    link_with : penrose_lib
)

subdir('tests')
subdir('demos')
