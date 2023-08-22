#ifndef PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP

#include <map>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderGraphInfo.hpp>

namespace Penrose {

    class DeviceContext;
    class PresentContext;
    class RenderOperator;

    class VkFramebuffer;
    class VkRenderSubgraph;
    class VkRenderTarget;
    class VkRenderTargetFactory;

    class RenderGraphExecutor {
    public:
        struct SubgraphEntry {
            VkRenderSubgraph *renderSubgraph;
            std::vector<RenderOperator *> renderOperators;
            VkFramebuffer *framebuffer;
        };

        RenderGraphExecutor(DeviceContext *deviceContext,
                            PresentContext *presentContext,
                            VkRenderTargetFactory *vkRenderTargetFactory,
                            RenderGraphInfo graph,
                            std::map<std::string, VkRenderTarget *> targets,
                            std::map<std::string, SubgraphEntry> subgraphs);
        ~RenderGraphExecutor();

        [[nodiscard]] std::vector<vk::SubmitInfo> execute(const vk::CommandBuffer &commandBuffer,
                                                          const vk::Semaphore &waitSemaphore,
                                                          const vk::Semaphore &signalSemaphore,
                                                          std::uint32_t frameIdx,
                                                          std::uint32_t imageIdx);

        void createFramebuffers();
        void destroyFramebuffers();

    private:
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
        VkRenderTargetFactory *_vkRenderTargetFactory;

        RenderGraphInfo _graph;
        std::map<std::string, VkRenderTarget *> _targets;
        std::map<std::string, SubgraphEntry> _subgraphs;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP
