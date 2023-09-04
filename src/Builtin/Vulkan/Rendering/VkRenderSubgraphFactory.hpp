#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSubgraphFactory : public Resource, public RenderSubgraphFactory {
    public:
        explicit VkRenderSubgraphFactory(ResourceSet *resources);
        ~VkRenderSubgraphFactory() override = default;

        [[nodiscard]] RenderSubgraph *makeRenderSubgraph(const RenderSubgraphInfo &subgraphInfo) override;

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<PresentContext> _presentContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
