#include "RenderGraphExecutor.hpp"

#include <utility>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderTarget.hpp>

#include "src/Builtin/Vulkan/Rendering/VkCommandRecording.hpp"
#include "src/Builtin/Vulkan/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    static constexpr const std::array<vk::PipelineStageFlags, 1> WAIT_DST_STAGE_MASK = {
            vk::PipelineStageFlagBits::eColorAttachmentOutput
    };

    RenderGraphExecutor::RenderGraphExecutor(VkLogicalDeviceContext *logicalDeviceContext,
                                             VkSwapchainManager *swapchainManager,
                                             std::map<std::string, RenderOperator *> operators,
                                             VkRenderTargetFactory *vkRenderTargetFactory,
                                             RenderGraphInfo graph,
                                             std::map<std::string, VkRenderTarget *> targets,
                                             std::map<std::string, SubgraphEntry> subgraphs)
            : _logicalDeviceContext(logicalDeviceContext),
              _swapchainManager(swapchainManager),
              _operators(std::move(operators)),
              _vkRenderTargetFactory(vkRenderTargetFactory),
              _graph(std::move(graph)),
              _targets(std::move(targets)),
              _subgraphs(std::move(subgraphs)) {
        //
    }

    RenderGraphExecutor::~RenderGraphExecutor() {
        this->destroyFramebuffers();

        for (const auto &[_, target]: this->_targets) {
            delete target;
        }

        for (const auto &[_, subgraph]: this->_subgraphs) {
            delete subgraph.renderSubgraph;
        }

        this->_targets.clear();
        this->_subgraphs.clear();
    }

    std::vector<vk::SubmitInfo> RenderGraphExecutor::execute(const vk::CommandBuffer &commandBuffer,
                                                             const vk::Semaphore &waitSemaphore,
                                                             const vk::Semaphore &signalSemaphore,
                                                             std::uint32_t frameIdx,
                                                             std::uint32_t imageIdx) {
        auto swapchainExtent = this->_swapchainManager->getSwapchain()->getExtent();
        auto swapchainSize = Size(swapchainExtent.width, swapchainExtent.height);

        if (this->_subgraphs.empty()) {
            return {};
        }

        std::vector<vk::SubmitInfo> submits;

        auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
        commandBuffer.begin(commandBufferBeginInfo);

        for (auto &[name, subgraph]: this->_subgraphs) {
            if (subgraph.framebuffer == nullptr) {
                continue;
            }

            auto renderPassBeginInfo = vk::RenderPassBeginInfo()
                    .setRenderPass(subgraph.renderSubgraph->getRenderPass())
                    .setFramebuffer(subgraph.framebuffer->getFramebuffer(imageIdx))
                    .setClearValues(subgraph.framebuffer->getClearValues())
                    .setRenderArea(subgraph.framebuffer->getRenderArea());

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            auto commandRecording = new VkCommandRecording(frameIdx, commandBuffer);
            auto &passes = subgraph.renderSubgraph->getSubgraphInfo().getPasses();

            for (std::uint32_t passIdx = 0; passIdx < passes.size(); passIdx++) {
                const auto &pass = passes.at(passIdx);

                if (passIdx != 0) {
                    commandBuffer.nextSubpass(vk::SubpassContents::eInline);
                }

                if (!pass.getOperator().has_value()) {
                    continue;
                }

                auto operatorIt = this->_operators.find(pass.getOperator()->getName());
                if (operatorIt == this->_operators.end()) {
                    throw EngineError(fmt::format("No such operator {}", pass.getOperator()->getName()));
                }

                auto context = RenderOperator::Context{
                        .subgraph = subgraph.renderSubgraph,
                        .subgraphPassIdx = passIdx,
                        .renderArea = subgraph.renderSubgraph->getSubgraphInfo()
                                .getRenderArea().value_or(swapchainSize),
                        .param = ParamsCollection::merge(operatorIt->second->getDefaults(),
                                                         pass.getOperator()->getParams())
                };

                operatorIt->second->execute(commandRecording, context);
            }

            commandBuffer.endRenderPass();
        }

        commandBuffer.end();

        return {
                vk::SubmitInfo()
                        .setCommandBuffers(commandBuffer)
                        .setWaitDstStageMask(WAIT_DST_STAGE_MASK)
                        .setWaitSemaphores(waitSemaphore)
                        .setSignalSemaphores(signalSemaphore)
        };
    }

    void RenderGraphExecutor::createFramebuffers() {
        for (auto &[targetName, target]: this->_targets) {
            auto targetInfo = this->_graph.getTargets().at(targetName);
            target = dynamic_cast<VkRenderTarget *>(this->_vkRenderTargetFactory->makeRenderTarget(
                    std::forward<decltype(targetInfo)>(targetInfo)
            ));
        }

        for (auto &[_, subgraph]: this->_subgraphs) {
            auto framebuffer = makeVkFramebuffer(this->_logicalDeviceContext,
                                                 this->_swapchainManager,
                                                 this->_targets,
                                                 subgraph.renderSubgraph->getRenderPass(),
                                                 subgraph.renderSubgraph->getSubgraphInfo());

            subgraph.framebuffer = framebuffer;
        }
    }

    void RenderGraphExecutor::destroyFramebuffers() {
        for (auto &[_, subgraph]: this->_subgraphs) {
            delete subgraph.framebuffer;
            subgraph.framebuffer = nullptr;
        }

        for (auto &[_, target]: this->_targets) {
            delete target;
            target = nullptr;
        }
    }
}

