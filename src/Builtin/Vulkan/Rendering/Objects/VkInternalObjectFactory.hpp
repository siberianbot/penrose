#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_INTERNAL_OBJECT_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_INTERNAL_OBJECT_FACTORY_HPP

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Graph/GraphInfo.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchain.hpp"

namespace Penrose {

    class VkInternalObjectFactory final: public Resource<VkInternalObjectFactory> {
    public:
        explicit VkInternalObjectFactory(const ResourceSet *resources);
        ~VkInternalObjectFactory() override = default;

        [[nodiscard]] vk::UniqueRenderPass makeRenderPass(const GraphInfo &graph, const VkSwapchain &swapchain);
        [[nodiscard]] vk::UniqueFramebuffer makeFramebuffer(
            const vk::RenderPass &renderPass, const std::vector<vk::ImageView> &imageViews, std::uint32_t width,
            std::uint32_t height
        );

    private:
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_INTERNAL_OBJECT_FACTORY_HPP
