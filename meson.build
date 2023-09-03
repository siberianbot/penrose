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
    dependency('vulkan'),
    dependency('imgui'),
    dependency('glm'),
]

incdir = include_directories('include')

src = [
    # Assets
    'src/Assets/AssetDictionary.cpp',
    'src/Assets/AssetLoader.cpp',
    'src/Assets/AssetReader.cpp',
    'src/Assets/AssetManager.cpp',
    'src/Assets/ImageAsset.cpp',
    'src/Assets/MeshAsset.cpp',
    'src/Assets/ShaderAsset.cpp',

    # Common
    'src/Common/ParamsCollection.cpp',

    # Core
    'src/Core/Engine.cpp',
    'src/Core/Log.cpp',

    # ECS
    'src/ECS/ECSManager.cpp',

    # Events
    'src/Events/Event.cpp',
    'src/Events/EventQueue.cpp',

    # Rendering
    'src/Rendering/DeviceContext.cpp',
    'src/Rendering/PresentContext.cpp',
    'src/Rendering/RenderContext.cpp',
    'src/Rendering/RenderGraphExecutor.cpp',
    'src/Rendering/RenderGraphExecutorProvider.cpp',
    'src/Rendering/RenderListBuilder.cpp',
    'src/Rendering/RenderManager.cpp',
    'src/Rendering/SurfaceManager.cpp',
    'src/Rendering/Utils.cpp',

    # Resources
    'src/Resources/ResourceSet.cpp',

    # Scene
    'src/Scene/SceneManager.cpp',
    'src/Scene/SceneNode.cpp',

    # Builtin / Rendering
    'src/Builtin/Rendering/ForwardSceneDrawRenderOperator.cpp',
    'src/Builtin/Rendering/ImGuiDrawRenderOperator.cpp',

    # Builtin / GLFW
    'src/Builtin/Glfw/GlfwBackend.cpp',

    'src/Builtin/Glfw/Rendering/GlfwSurface.cpp',
    'src/Builtin/Glfw/Rendering/GlfwSurfaceController.cpp',

    # Builtin / ImGui
    'src/Builtin/ImGui/ImGuiBackend.cpp',

    # Builtin / Vulkan
    'src/Builtin/Vulkan/VulkanBackend.cpp',

    'src/Builtin/Vulkan/Assets/VkImageAssetFactory.cpp',
    'src/Builtin/Vulkan/Assets/VkMeshAssetFactory.cpp',
    'src/Builtin/Vulkan/Assets/VkShaderAssetFactory.cpp',

    'src/Builtin/Vulkan/Rendering/VkBuffer.cpp',
    'src/Builtin/Vulkan/Rendering/VkBufferFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkCommandRecording.cpp',
    'src/Builtin/Vulkan/Rendering/VkDescriptor.cpp',
    'src/Builtin/Vulkan/Rendering/VkFramebuffer.cpp',
    'src/Builtin/Vulkan/Rendering/VkImage.cpp',
    'src/Builtin/Vulkan/Rendering/VkImageFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkPipeline.cpp',
    'src/Builtin/Vulkan/Rendering/VkPipelineFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSubgraph.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderTarget.cpp',
    'src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkSampler.cpp',
    'src/Builtin/Vulkan/Rendering/VkSamplerFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkShader.cpp',
    'src/Builtin/Vulkan/Rendering/VkShaderFactory.cpp',
    'src/Builtin/Vulkan/Rendering/VkUtils.cpp',
]

cpp_args = [
    '-DSTB_IMAGE_IMPLEMENTATION',
    '-DTINYOBJLOADER_IMPLEMENTATION',
    '-DGLM_FORCE_DEPTH_ZERO_TO_ONE'
]

penrose = library(
    'penrose',
    src,
    dependencies: deps,
    cpp_args: cpp_args,
    include_directories: incdir
)

penrose_dep = declare_dependency(
    dependencies: deps,
    include_directories: incdir,
    link_with: penrose
)

subdir('tests')