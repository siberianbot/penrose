#include "DeviceContext.hpp"

#include <array>
#include <set>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    vk::CommandPool DeviceContext::createCommandPool() {
        auto createInfo = vk::CommandPoolCreateInfo()
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(this->_physicalDeviceContext->getGraphicsFamilyIdx());

        return this->_logicalDeviceContext->getHandle().createCommandPool(createInfo);
    }

    vk::DescriptorPool DeviceContext::createDescriptorPool() {
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

        auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo()
                .setMaxSets(maxSets)
                .setPoolSizes(descriptorPoolSizes);

        return this->_logicalDeviceContext->getHandle().createDescriptorPool(descriptorPoolCreateInfo);
    }

    DeviceContext::DeviceContext(ResourceSet *resources)
            : _vulkanBackend(resources->getLazy<VulkanBackend>()),
              _vkSurfaceProvider(resources->getLazy<VkSurfaceProvider>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->getLazy<VkPhysicalDeviceContext>()) {
        //
    }

    void DeviceContext::init() {

        try {
            this->_commandPool = this->createCommandPool();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create command pool"));
        }

        try {
            this->_descriptorPool = this->createDescriptorPool();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create descriptor pool"));
        }
    }

    void DeviceContext::destroy() {
        if (this->_descriptorPool.has_value()) {
            this->_logicalDeviceContext->getHandle().destroy(*this->_descriptorPool);
            this->_descriptorPool = std::nullopt;
        }

        if (this->_commandPool.has_value()) {
            this->_logicalDeviceContext->getHandle().destroy(*this->_commandPool);
            this->_commandPool = std::nullopt;
        }
    }
}
