#include "VkRenderGraphExecutor.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkCommandRecording.hpp"

namespace Penrose {

    VkRenderGraphExecutor::VkRenderGraphExecutor(const ResourceSet *resources)
            : _renderOperators(resources->get<RenderOperator>()),
              _swapchainManager(resources->get<VkSwapchainManager>()) {
        //
    }

    void VkRenderGraphExecutor::init() {
        for (const auto &renderOperator: this->_renderOperators) {
            this->_renderOperatorsMap.emplace(renderOperator->getName(), renderOperator);
        }
    }

    void VkRenderGraphExecutor::destroy() {
        this->_renderOperatorsMap.clear();
    }

    std::vector<VkRenderGraphExecutor::Submit> VkRenderGraphExecutor::execute(VkRenderGraphContext *graphContext,
                                                                              const vk::CommandBuffer &commandBuffer,
                                                                              const vk::Semaphore &waitSemaphore,
                                                                              const vk::Semaphore &signalSemaphore,
                                                                              std::uint32_t frameIdx,
                                                                              std::uint32_t imageIdx) {
        auto swapchainExtent = this->_swapchainManager->getSwapchain()->getExtent();
        auto swapchainSize = Size(swapchainExtent.width, swapchainExtent.height);

        auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
        commandBuffer.begin(commandBufferBeginInfo);

        for (const auto &[subgraphName, subgraphInfo]: graphContext->getGraphInfo().getSubgraphs()) {
            auto subgraph = graphContext->getOrCreateSubgraph(subgraphName);
            auto framebuffer = graphContext->getOrCreateFramebuffer(subgraphName);

            auto renderPassBeginInfo = vk::RenderPassBeginInfo()
                    .setRenderPass(subgraph->getRenderPass())
                    .setFramebuffer(framebuffer->getFramebuffer(imageIdx))
                    .setClearValues(framebuffer->getClearValues())
                    .setRenderArea(framebuffer->getRenderArea());

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            for (std::uint32_t passIdx = 0; passIdx < subgraphInfo.getPasses().size(); passIdx++) {
                const auto &pass = subgraphInfo.getPasses().at(passIdx);

                if (passIdx != 0) {
                    commandBuffer.nextSubpass(vk::SubpassContents::eInline);
                }

                if (!pass.getOperator().has_value()) {
                    continue;
                }

                auto operatorIt = this->_renderOperatorsMap.find(pass.getOperator()->getName());
                if (operatorIt == this->_renderOperatorsMap.end()) {
                    throw EngineError("No such operator {}", pass.getOperator()->getName());
                }

                auto context = RenderOperator::Context{
                        .subgraph = subgraph.get(),
                        .subgraphPassIdx = passIdx,
                        .renderArea = subgraphInfo.getRenderArea().value_or(swapchainSize),
                        .param = ParamsCollection::merge(operatorIt->second->getDefaults(),
                                                         pass.getOperator()->getParams())
                };

                auto commandRecording = new VkCommandRecording(frameIdx, commandBuffer);

                operatorIt->second->execute(commandRecording, context);

                delete commandRecording;
            }

            commandBuffer.endRenderPass();
        }

        commandBuffer.end();

        return {
                Submit{
                        .commandBuffer = commandBuffer,
                        .waitSemaphores = {waitSemaphore},
                        .signalSemaphores = {signalSemaphore}
                }
        };
    }
}
