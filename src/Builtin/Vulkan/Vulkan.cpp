#include <Penrose/Builtin/Vulkan/Vulkan.hpp>

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
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    ResourceSet &addVulkan(ResourceSet &resources) {

        resources.add<VulkanBackend>().group(ResourceGroup::Backend).implements<Initializable>().done();

        resources.add<VkPhysicalDeviceContext>().group(ResourceGroup::Rendering).done();

        resources.add<VkLogicalDeviceContext>().group(ResourceGroup::Rendering).done();

        resources.add<VkCommandManager>().group(ResourceGroup::Rendering).done();

        resources.add<VkDescriptorPoolManager>().group(ResourceGroup::Rendering).done();

        resources.add<VkMemoryAllocator>().group(ResourceGroup::Rendering).done();

        resources.add<VkSwapchainPreferencesProvider>().group(ResourceGroup::Rendering).done();

        resources.add<VkSwapchainFactory>().group(ResourceGroup::Rendering).done();

        resources.add<VkSwapchainManager>().group(ResourceGroup::Rendering).done();

        resources.add<VkBufferFactory>().group(ResourceGroup::Rendering).implements<BufferFactory>().done();

        resources.add<VkImageFactory>().group(ResourceGroup::Rendering).implements<ImageFactory>().done();

        resources.add<VkPipelineFactory>().group(ResourceGroup::Rendering).implements<PipelineFactory>().done();

        resources.add<VkSamplerFactory>().group(ResourceGroup::Rendering).implements<SamplerFactory>().done();

        resources.add<VkShaderFactory>().group(ResourceGroup::Rendering).implements<ShaderFactory>().done();

        resources.add<VkRenderingObjectManager>()
            .group(ResourceGroup::Rendering)
            .implements<RenderingObjectManager>()
            .done();

        resources.add<VkRenderSubgraphFactory>().group(ResourceGroup::Rendering).done();

        resources.add<VkRenderTargetFactory>().group(ResourceGroup::Rendering).done();

        resources.add<VkFramebufferFactory>().group(ResourceGroup::Rendering).done();

        resources.add<VkRenderGraphContextManager>()
            .group(ResourceGroup::Rendering)
            .implements<RenderGraphHook>()
            .done();

        resources.add<VkRenderGraphExecutor>().group(ResourceGroup::Rendering).done();

        resources.add<VkRenderSystem>().group(ResourceGroup::Rendering).implements<RenderSystem>().done();

        return resources;
    }
}
