#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSubgraphFactory : public Resource {
    public:
        explicit VkRenderSubgraphFactory(ResourceSet *resources);
        ~VkRenderSubgraphFactory() override = default;

        [[nodiscard]] VkRenderSubgraph *makeRenderSubgraph(RenderSubgraphInfo &&subgraphInfo);

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkSwapchainManager> _swapchainManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
