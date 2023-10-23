#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP

#include <map>
#include <string>

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class VkRenderSubgraph;
    class VkRenderTarget;

    class VkFramebufferFactory : public Resource<VkFramebufferFactory> {
    public:
        explicit VkFramebufferFactory(ResourceSet *resources);
        ~VkFramebufferFactory() override = default;

        [[nodiscard]] VkFramebuffer *makeFramebuffer(const std::map<std::string, VkRenderTarget *> &targets,
                                                     VkRenderSubgraph *subgraph);

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkSwapchainManager> _swapchainManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP
