#include <Penrose/Builtin/Vulkan.hpp>

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

        resources.add<VulkanBackend>(resources.getBeginIterator());

        resources.add<VkPhysicalDeviceContext>();
        resources.add<VkLogicalDeviceContext>();
        resources.add<VkMemoryAllocator>();
        resources.add<VkCommandManager>();
        resources.add<VkDescriptorPoolManager>();

        resources.add<VkSwapchainPreferencesProvider>();
        resources.add<VkSwapchainFactory>();
        resources.add<VkSwapchainManager>();

        resources.add<VkBufferFactory, BufferFactory>();
        resources.add<VkImageFactory, ImageFactory>();
        resources.add<VkPipelineFactory, PipelineFactory>();
        resources.add<VkSamplerFactory, SamplerFactory>();
        resources.add<VkShaderFactory, ShaderFactory>();
        resources.add<VkRenderingObjectManager, RenderingObjectManager>();

        resources.add<VkRenderSubgraphFactory>();
        resources.add<VkRenderTargetFactory>();
        resources.add<VkFramebufferFactory>();

        resources.add<VkRenderGraphContextManager, RenderGraphHook>();
        resources.add<VkRenderGraphExecutor>();

        resources.add<VkRenderSystem, RenderSystem>();

        return resources;
    }
}
