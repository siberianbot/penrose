#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSubgraphFactory : public Resource, public RenderSubgraphFactory {
    public:
        explicit VkRenderSubgraphFactory(ResourceSet *resources);
        ~VkRenderSubgraphFactory() override = default;

        [[nodiscard]] RenderSubgraph *makeRenderSubgraph(RenderSubgraphInfo &&subgraphInfo) override;

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkSwapchainManager> _swapchainManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
