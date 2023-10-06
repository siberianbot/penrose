#include "VkDescriptorPoolManager.hpp"

#include <algorithm>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr static const std::string_view VK_DESCRIPTOR_POOL_MANAGER_TAG = "VkDescriptorPoolManager";

    VkDescriptorPoolManager::VkDescriptorPoolManager(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()) {
        //
    }

    void VkDescriptorPoolManager::init() {
        this->_log->writeInfo(VK_DESCRIPTOR_POOL_MANAGER_TAG, "Initializing descriptor pool manager");

        try {
            this->_state = this->createDescriptorPool();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create descriptor pool"));
        }
    }

    void VkDescriptorPoolManager::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        this->_logicalDeviceContext->getHandle().destroy(this->_state->descriptorPool);
        this->_state = std::nullopt;
    }

    VkDescriptorPoolManager::DescriptorSets VkDescriptorPoolManager::createDescriptorSets(
            const vk::DescriptorSetLayout &layout) {
        auto layouts = std::vector<vk::DescriptorSetLayout>(INFLIGHT_FRAME_COUNT, layout);
        auto allocateInfo = vk::DescriptorSetAllocateInfo()
                .setSetLayouts(layouts)
                .setDescriptorPool(this->_state->descriptorPool);

        VkDescriptorPoolManager::DescriptorSets descriptorSetsArray;
        auto descriptorSets = this->_logicalDeviceContext->getHandle().allocateDescriptorSets(allocateInfo);

        std::move(descriptorSets.begin(), descriptorSets.end(), descriptorSetsArray.begin());

        return descriptorSetsArray;
    }

    void VkDescriptorPoolManager::destroyDescriptorSets(const VkDescriptorPoolManager::DescriptorSets &descriptorSets) {
        this->_logicalDeviceContext->getHandle().free(this->_state->descriptorPool, descriptorSets);
    }

    VkDescriptorPoolManager::State VkDescriptorPoolManager::createDescriptorPool() {
        const auto descriptorCount = 1024;
        const auto maxSets = 1024;

        auto descriptorPoolSizes = {
                vk::DescriptorPoolSize(vk::DescriptorType::eSampler, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, descriptorCount),
                vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, descriptorCount),
        };

        auto createInfo = vk::DescriptorPoolCreateInfo()
                .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
                .setMaxSets(maxSets)
                .setPoolSizes(descriptorPoolSizes);

        return {
                .descriptorPool = this->_logicalDeviceContext->getHandle().createDescriptorPool(createInfo)
        };
    }
}
