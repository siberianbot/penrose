#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERER_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERER_CONTEXT_HPP

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/RendererContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkRenderContext.hpp"

namespace Penrose {

    class VkRendererContext final: public RendererContext {
    public:
        explicit VkRendererContext(VkRenderContext *renderContext, ResourceProxy<Log> log);
        ~VkRendererContext() override = default;

        void executeGraph(
            const std::initializer_list<TargetInfo> &targets, const GraphInfo &graph,
            const RendererFunctionMap &functions
        ) override;

    private:
        VkRenderContext *_renderContext;

        ResourceProxy<Log> _log;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERER_CONTEXT_HPP
