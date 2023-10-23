#include <Penrose/Builtin/Vulkan.hpp>

#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkFramebufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContextManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphExecutor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderingObjectManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSystem.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSamplerFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkShaderFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainPreferencesProvider.hpp"

namespace Penrose {

    ResourceSet &addVulkan(ResourceSet &resources) {

        resources.add<VulkanBackend>()
                .implements<Initializable>()
                .before<SurfaceManager>()
                .done();

        resources.add<VkPhysicalDeviceContext>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkLogicalDeviceContext>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkCommandManager>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkDescriptorPoolManager>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkMemoryAllocator>()
                .done();

        resources.add<VkSwapchainPreferencesProvider>()
                .done();

        resources.add<VkSwapchainFactory>()
                .before<RenderManager>()
                .done();

        resources.add<VkSwapchainManager>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkBufferFactory>()
                .implements<BufferFactory>()
                .done();

        resources.add<VkImageFactory>()
                .implements<ImageFactory>()
                .done();

        resources.add<VkPipelineFactory>()
                .implements<Initializable>()
                .implements<PipelineFactory>()
                .before<RenderManager>()
                .done();

        resources.add<VkSamplerFactory>()
                .implements<SamplerFactory>()
                .done();

        resources.add<VkShaderFactory>()
                .implements<ShaderFactory>()
                .done();

        resources.add<VkRenderingObjectManager>()
                .implements<RenderingObjectManager>()
                .done();

        resources.add<VkRenderSubgraphFactory>()
                .done();

        resources.add<VkRenderTargetFactory>()
                .done();

        resources.add<VkFramebufferFactory>()
                .done();

        resources.add<VkRenderGraphContextManager>()
                .implements<Initializable>()
                .implements<RenderGraphHook>()
                .before<RenderManager>()
                .done();

        resources.add<VkRenderGraphExecutor>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkRenderSystem>()
                .implements<Initializable>()
                .implements<Runnable>()
                .implements<RenderSystem>()
                .before<RenderManager>()
                .done();

        return resources;
    }
}
