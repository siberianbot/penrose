#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class VkRenderSubgraphFactory : public Resource<VkRenderSubgraphFactory, ResourceGroup::Rendering> {
    public:
        explicit VkRenderSubgraphFactory(ResourceSet *resources);
        ~VkRenderSubgraphFactory() override = default;

        [[nodiscard]] VkRenderSubgraph *makeRenderSubgraph(RenderSubgraphInfo &&subgraphInfo);

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkSwapchainManager> _swapchainManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
