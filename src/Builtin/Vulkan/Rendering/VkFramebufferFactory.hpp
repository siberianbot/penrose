#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP

#include <map>
#include <string>

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSubgraph;
    class VkRenderTarget;

    class VkFramebufferFactory : public Resource {
    public:
        explicit VkFramebufferFactory(ResourceSet *resources);
        ~VkFramebufferFactory() override = default;

        [[nodiscard]] VkFramebuffer *makeFramebuffer(const std::map<std::string, VkRenderTarget *> &targets,
                                                     VkRenderSubgraph *subgraph);

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkSwapchainManager> _swapchainManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_FACTORY_HPP
