project(
    'penrose',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20'
    ])

deps = [
    dependency('fmt'),
    dependency('glfw3'),
    dependency('glm'),
    dependency('imgui'),
    dependency('libxml-2.0'),
    dependency('libxml++'),
    dependency('vulkan'),
]

incdir = include_directories('include')

src = [
    # Core
    'src/Engine.cpp',

    # Assets
    'src/Assets/AssetDictionary.cpp',
    'src/Assets/AssetLoader.cpp',
    'src/Assets/AssetReader.cpp',
    'src/Assets/AssetManager.cpp',
    'src/Assets/ImageAsset.cpp',
    'src/Assets/MeshAsset.cpp',
    'src/Assets/ShaderAsset.cpp',
    'src/Assets/UILayoutAsset.cpp',

    # Common
    'src/Common/Log.cpp',
    'src/Common/ParamsCollection.cpp',
    'src/Common/StdOutLogSink.cpp',

    # ECS
    'src/ECS/EntityManager.cpp',
    'src/ECS/SystemManager.cpp',

    # Input
    'src/Input/Input.cpp',
    'src/Input/InputHandler.cpp',

    # Performance
    'src/Performance/Profiler.cpp',

    # Rendering
    'src/Rendering/DefaultDrawableProvider.cpp',
    'src/Rendering/DefaultViewProvider.cpp',
    'src/Rendering/RenderGraphContext.cpp',
    'src/Rendering/RenderListBuilder.cpp',
    'src/Rendering/RenderManager.cpp',
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
    'src/UI/ValueProxy.cpp',

    # Builtin / Penrose
    'src/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.cpp',

    # Builtin / GLFW
    'src/Builtin/Glfw/Glfw.cpp',
    'src/Builtin/Glfw/GlfwBackend.cpp',
    'src/Builtin/Glfw/Rendering/GlfwSurface.cpp',
    'src/Builtin/Glfw/Rendering/GlfwSurfaceController.cpp',
    'src/Builtin/Glfw/Utils/InputUtils.cpp',

    # Builtin / ImGui
    'src/Builtin/ImGui/ImGui.cpp',
    'src/Builtin/ImGui/ImGuiBackend.cpp',
    'src/Builtin/ImGui/Rendering/VkImGuiRenderOperator.cpp',
    'src/Builtin/ImGui/UI/ImGuiUIInstanceVisitor.cpp',

    # Builtin / Vulkan
    'src/Builtin/Vulkan/Vulkan.cpp',
    'src/Builtin/Vulkan/VulkanBackend.cpp',
    'src/Builtin/Vulkan/Rendering/VkBuffer.cpp',
    'src/Builtin/Vulkan/Rendering/VkBufferFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkCommandManager.cpp',
    'src/Builtin/Vulkan/Rendering/VkCommandRecording.cpp',
    'src/Builtin/Vulkan/Rendering/VkDescriptor.cpp',
    'src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.cpp',
    'src/Builtin/Vulkan/Rendering/VkFramebuffer.cpp',
    'src/Builtin/Vulkan/Rendering/VkFramebufferFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkImage.cpp',
    'src/Builtin/Vulkan/Rendering/VkImageFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.cpp',
    'src/Builtin/Vulkan/Rendering/VkMemoryAllocator.cpp',
    'src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.cpp',
    'src/Builtin/Vulkan/Rendering/VkPipeline.cpp',
    'src/Builtin/Vulkan/Rendering/VkPipelineFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderGraphContext.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderGraphContextManager.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderGraphExecutor.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderingObjectManager.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSubgraph.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSystem.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderTarget.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkSampler.cpp',
    'src/Builtin/Vulkan/Rendering/VkSamplerFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkShader.cpp',
    'src/Builtin/Vulkan/Rendering/VkShaderFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkSwapchain.cpp',
    'src/Builtin/Vulkan/Rendering/VkSwapchainFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkSwapchainManager.cpp',
    'src/Builtin/Vulkan/Rendering/VkSwapchainPreferencesProvider.cpp',
    'src/Builtin/Vulkan/Rendering/VkUtils.cpp',
]

base_cpp_args = [
    '-DGLM_FORCE_DEPTH_ZERO_TO_ONE'
]

penrose_api_value = '__attribute__ ((visibility ("default")))'

if host_machine.system() == 'windows'
    penrose_api_value = get_option('default_library') != 'static' ? '__declspec(dllexport)' : ''
endif

lib_cpp_args = base_cpp_args + [
    '-DPENROSE_API=@0@'.format(penrose_api_value)
]

if host_machine.system() == 'windows'
    penrose_api_value = get_option('default_library') != 'static' ? '__declspec(dllimport)' : ''
endif

dep_cpp_args = base_cpp_args + [
    '-DPENROSE_API=@0@'.format(penrose_api_value)
]

penrose_lib = library(
    'penrose',
    src,
    dependencies: deps,
    cpp_args: lib_cpp_args,
    include_directories: incdir,
    gnu_symbol_visibility: 'hidden'
)

penrose_dep = declare_dependency(
    dependencies: deps,
    compile_args: dep_cpp_args,
    include_directories: incdir,
    link_with: penrose_lib
)

subdir('tests')