#include "RenderGraphExecutor.hpp"

#include <utility>

#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderTarget.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Rendering/VkRenderPass.hpp"
#include "src/Builtin/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    static constexpr const std::array<vk::PipelineStageFlags, 1> WAIT_DST_STAGE_MASK = {
            vk::PipelineStageFlagBits::eColorAttachmentOutput
    };

    RenderGraphExecutor::RenderGraphExecutor(DeviceContext *deviceContext,
                                             PresentContext *presentContext,
                                             VkRenderTargetFactory *vkRenderTargetFactory,
                                             RenderGraph graph,
                                             std::map<std::string, VkRenderTarget *> targets,
                                             std::map<std::string, SubgraphEntry> subgraphs)
            : _deviceContext(deviceContext),
              _presentContext(presentContext),
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
            for (const auto &renderOperator: subgraph.renderOperators) {
                delete renderOperator;
            }

            delete subgraph.renderPass;
        }

        this->_targets.clear();
        this->_subgraphs.clear();
    }

    std::vector<vk::SubmitInfo> RenderGraphExecutor::execute(const vk::CommandBuffer &commandBuffer,
                                                             const vk::Semaphore &waitSemaphore,
                                                             const vk::Semaphore &signalSemaphore,
                                                             std::uint32_t frameIdx,
                                                             std::uint32_t imageIdx) {
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
                    .setRenderPass(subgraph.renderPass->getRenderPass())
                    .setFramebuffer(subgraph.framebuffer->getFramebuffer(imageIdx))
                    .setClearValues(subgraph.framebuffer->getClearValues())
                    .setRenderArea(subgraph.framebuffer->getRenderArea());

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            auto executionContext = RenderOperator::Context{
                    .frameIdx = frameIdx,
                    .renderArea = subgraph.framebuffer->getRenderArea(),
                    .commandBuffer = commandBuffer
            };

            for (auto it = subgraph.renderOperators.begin(); it != subgraph.renderOperators.end(); it++) {
                if (it != subgraph.renderOperators.begin()) {
                    commandBuffer.nextSubpass(vk::SubpassContents::eInline);
                }

                if (*it == nullptr) {
                    continue;
                }

                (*it)->execute(executionContext);
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
            target = dynamic_cast<VkRenderTarget *>(this->_vkRenderTargetFactory->makeRenderTarget(targetInfo));
        }

        for (auto &[_, subgraph]: this->_subgraphs) {
            auto framebuffer = makeVkFramebuffer(this->_deviceContext,
                                                 this->_presentContext,
                                                 this->_targets,
                                                 subgraph.renderPass->getRenderPass(),
                                                 subgraph.subgraph);

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

