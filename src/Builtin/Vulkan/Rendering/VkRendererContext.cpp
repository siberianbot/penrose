#include "VkRendererContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkCommandRecorder.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkRendererContext";

    VkRendererContext::VkRendererContext(VkRenderContext *renderContext, ResourceProxy<Log> log)
        : _renderContext(renderContext),
          _log(std::move(log)) {
        //
    }

    void VkRendererContext::executeGraph(
        const std::initializer_list<TargetInfo> &targets, const GraphInfo &graph, const RendererFunctionMap &functions
    ) {
        const auto pass = this->_renderContext->usePass(graph);
        auto clearValues = std::vector<vk::ClearValue>(graph.attachments.size());

        std::ranges::transform(graph.attachments, clearValues.begin(), [](const auto &attachment) {
            return mapClearValue(attachment.clearValue);
        });

        vk::Rect2D rect;
        if (graph.area.has_value()) {
            rect = mapRect(*graph.area);
        } else {
            const auto extent = this->_renderContext->getSwapchain().extent;

            rect = vk::Rect2D(vk::Offset2D(0, 0), extent);
        }

        const auto framebuffer = this->_renderContext->useFramebuffer(targets, graph, rect.extent);
        const auto &commandBuffer = this->_renderContext->getCurrentCommandBuffer();

        commandBuffer.beginRenderPass(
            vk::RenderPassBeginInfo()
                .setRenderPass(pass)
                .setFramebuffer(framebuffer)
                .setRenderArea(rect)
                .setClearValues(clearValues),
            vk::SubpassContents::eInline
        );

        for (std::uint32_t idx = 0; idx < graph.passes.size(); idx++) {
            if (idx > 0) {
                commandBuffer.nextSubpass(vk::SubpassContents::eInline);
            }

            if (graph.passes.at(idx).function.has_value()) {
                const auto function = *graph.passes.at(idx).function;

                const auto it = functions.find(function);

                if (it == functions.end()) {
                    this->_log->writeError(
                        TAG, "Graph pass #{} have renderer function {}, which is not exported by current renderer", idx,
                        function
                    );
                } else {
                    try {
                        auto commandRecorder = std::make_unique<VkCommandRecorder>(
                            this->_renderContext, commandBuffer, pass, idx
                        );

                        it->second(commandRecorder.get());
                    } catch (const std::exception &error) {
                        this->_log->writeError(
                            TAG, "Graph pass #{}, error with function {}: {}", idx, function, error.what()
                        );
                    }
                }
            }
        }

        commandBuffer.endRenderPass();
    }
}
