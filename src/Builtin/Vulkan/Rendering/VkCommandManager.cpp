#include "VkCommandManager.hpp"

#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr static const std::string_view VK_COMMAND_MANAGER_TAG = "VkCommandManager";

    VkCommandManager::VkCommandManager(ResourceSet *resources)
            : _log(resources->get<Log>()),
              _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->get<VkPhysicalDeviceContext>()) {
        //
    }

    void VkCommandManager::init() {
        this->_log->writeInfo(VK_COMMAND_MANAGER_TAG, "Initializing command manager");

        try {
            this->_state = this->createCommandManagerState();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create command pools"));
        }
    }

    void VkCommandManager::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        this->_logicalDeviceContext->getHandle().free(this->_state->graphicsCommandPool,
                                                      this->_state->graphicsCommandBuffers);

        this->_logicalDeviceContext->getHandle().destroy(this->_state->graphicsCommandPool);
        this->_logicalDeviceContext->getHandle().destroy(this->_state->transferCommandPool);

        this->_state = std::nullopt;
    }

    void VkCommandManager::executeGraphicsOnce(const VkCommandManager::Command &command) {
        this->executeOnce(this->_state->graphicsCommandPool, this->_logicalDeviceContext->getGraphicsQueue(), command);
    }

    void VkCommandManager::executeTransferOnce(const VkCommandManager::Command &command) {
        this->executeOnce(this->_state->transferCommandPool, this->_logicalDeviceContext->getTransferQueue(), command);
    }

    VkCommandManager::State VkCommandManager::createCommandManagerState() {
        State state;

        auto graphicsCreateInfo = vk::CommandPoolCreateInfo()
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(this->_physicalDeviceContext->getGraphicsFamilyIdx());

        state.graphicsCommandPool = this->_logicalDeviceContext->getHandle().createCommandPool(graphicsCreateInfo);

        auto graphicsCommandBuffersAllocateInfo = vk::CommandBufferAllocateInfo()
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(state.graphicsCommandPool)
                .setCommandBufferCount(INFLIGHT_FRAME_COUNT);

        auto graphicsCommandBuffers = this->_logicalDeviceContext->getHandle()
                .allocateCommandBuffers(graphicsCommandBuffersAllocateInfo);

        std::move(graphicsCommandBuffers.begin(), graphicsCommandBuffers.end(), state.graphicsCommandBuffers.begin());

        auto transferCreateInfo = vk::CommandPoolCreateInfo()
                .setQueueFamilyIndex(this->_physicalDeviceContext->getTransferFamilyIdx());

        state.transferCommandPool = this->_logicalDeviceContext->getHandle().createCommandPool(transferCreateInfo);

        return state;
    }

    void VkCommandManager::executeOnce(const vk::CommandPool &commandPool, const vk::Queue &queue,
                                       const VkCommandManager::Command &command) {
        auto allocateInfo = vk::CommandBufferAllocateInfo()
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(commandPool)
                .setCommandBufferCount(1);

        auto commandBuffers = this->_logicalDeviceContext->getHandle().allocateCommandBuffers(allocateInfo);
        auto commandBuffer = commandBuffers.at(0);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        commandBuffer.begin(beginInfo);

        command(commandBuffer);

        commandBuffer.end();

        auto submit = vk::SubmitInfo().setCommandBuffers(commandBuffer);

        queue.submit(submit);
        queue.waitIdle();

        this->_logicalDeviceContext->getHandle().free(commandPool, commandBuffers);
    }
}
