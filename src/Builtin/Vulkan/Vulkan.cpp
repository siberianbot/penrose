#include <Penrose/Builtin/Vulkan.hpp>

#include <Penrose/Rendering/RenderManager.hpp>

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

        resources.add<VulkanBackend, ResourceGroup::Backend>()
                .implements<Initializable>()
                .done();

        resources.add<VkPhysicalDeviceContext, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkLogicalDeviceContext, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkCommandManager, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkDescriptorPoolManager, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkMemoryAllocator, ResourceGroup::Rendering>()
                .done();

        resources.add<VkSwapchainPreferencesProvider, ResourceGroup::Rendering>()
                .done();

        resources.add<VkSwapchainFactory, ResourceGroup::Rendering>()
                .before<RenderManager>()
                .done();

        resources.add<VkSwapchainManager, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkBufferFactory, ResourceGroup::Rendering>()
                .implements<BufferFactory>()
                .done();

        resources.add<VkImageFactory, ResourceGroup::Rendering>()
                .implements<ImageFactory>()
                .done();

        resources.add<VkPipelineFactory, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .implements<PipelineFactory>()
                .before<RenderManager>()
                .done();

        resources.add<VkSamplerFactory, ResourceGroup::Rendering>()
                .implements<SamplerFactory>()
                .done();

        resources.add<VkShaderFactory, ResourceGroup::Rendering>()
                .implements<ShaderFactory>()
                .done();

        resources.add<VkRenderingObjectManager, ResourceGroup::Rendering>()
                .implements<RenderingObjectManager>()
                .done();

        resources.add<VkRenderSubgraphFactory, ResourceGroup::Rendering>()
                .done();

        resources.add<VkRenderTargetFactory, ResourceGroup::Rendering>()
                .done();

        resources.add<VkFramebufferFactory, ResourceGroup::Rendering>()
                .done();

        resources.add<VkRenderGraphContextManager, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .implements<RenderGraphHook>()
                .before<RenderManager>()
                .done();

        resources.add<VkRenderGraphExecutor, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .before<RenderManager>()
                .done();

        resources.add<VkRenderSystem, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .implements<Runnable>()
                .implements<RenderSystem>()
                .before<RenderManager>()
                .done();

        return resources;
    }
}
