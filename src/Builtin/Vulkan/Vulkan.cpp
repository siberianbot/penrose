#include <Penrose/Builtin/Vulkan/Vulkan.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkInternalObjectFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceSelector.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSamplerFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkShaderFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchainFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSystem.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSurfacePreferencesProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    void addVulkan(ResourceSet &resources) {

        resources.add<VulkanBackend>().group(ResourceGroup::Backend).implements<Initializable>().done();

        resources.add<VkMemoryAllocator>().group(ResourceGroup::Rendering).done();

        resources.add<VkBufferFactory>().group(ResourceGroup::Rendering).implements<BufferFactory>().done();
        resources.add<VkImageFactory>().group(ResourceGroup::Rendering).implements<ImageFactory>().done();
        resources.add<VkPipelineFactory>().group(ResourceGroup::Rendering).implements<PipelineFactory>().done();
        resources.add<VkSamplerFactory>().group(ResourceGroup::Rendering).implements<SamplerFactory>().done();
        resources.add<VkShaderFactory>().group(ResourceGroup::Rendering).implements<ShaderFactory>().done();

        resources.add<VkInternalObjectFactory>().group(ResourceGroup::Rendering).done();
        resources.add<VkLogicalDeviceFactory>().group(ResourceGroup::Rendering).done();
        resources.add<VkPhysicalDeviceSelector>().group(ResourceGroup::Rendering).done();
        resources.add<VkSwapchainFactory>().group(ResourceGroup::Rendering).done();
        resources.add<VkSurfacePreferencesProvider>().group(ResourceGroup::Rendering).done();

        resources.add<VkRenderSystem>()
            .group(ResourceGroup::Rendering)
            .implements<RenderSystem>()
            .implements<VkPhysicalDeviceProvider>()
            .implements<VkLogicalDeviceProvider>()
            .done();
    }
}
