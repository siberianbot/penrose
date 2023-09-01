#include "DeviceContext.hpp"

#include <array>
#include <set>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    static constexpr const std::array<std::string_view, 2> REQUIRED_DEVICE_EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME
    };

    DeviceContext::PhysicalDeviceProxy DeviceContext::selectPhysicalDevice() {
        auto devices = this->_vulkanBackend->getInstance().enumeratePhysicalDevices();
        auto surface = this->_vkSurfaceProvider->getVkSurfaceFor(this->_surfaceManager->getSurface());

        for (const auto &device: devices) {
            auto queueFamilyProperties = device.getQueueFamilyProperties();

            std::optional<uint32_t> graphicsIdx;
            std::optional<uint32_t> presentIdx;

            for (std::uint32_t idx = 0; idx < queueFamilyProperties.size(); idx++) {
                if (graphicsIdx.has_value() && presentIdx.has_value()) {
                    break;
                }

                if (queueFamilyProperties[idx].queueFlags & vk::QueueFlagBits::eGraphics) {
                    graphicsIdx = idx;
                }

                if (device.getSurfaceSupportKHR(idx, surface)) {
                    presentIdx = idx;
                }
            }

            if (!graphicsIdx.has_value() || !presentIdx.has_value()) {
                continue;
            }

            auto extensionProperties = device.enumerateDeviceExtensionProperties();
            auto missingExtensions = std::vector<std::string_view>(REQUIRED_DEVICE_EXTENSIONS.begin(),
                                                                   REQUIRED_DEVICE_EXTENSIONS.end());

            for (const auto &extension: extensionProperties) {
                missingExtensions.erase(
                        std::remove(missingExtensions.begin(), missingExtensions.end(), extension.extensionName),
                        missingExtensions.end());
            }

            if (!missingExtensions.empty()) {
                continue;
            }

            return {
                    .handle = device,
                    .graphicsQueueFamilyIdx = graphicsIdx.value(),
                    .presentQueueFamilyIdx = presentIdx.value(),
                    .features = device.getFeatures()
            };
        }

        throw EngineError("No suitable physical device available");
    }

    DeviceContext::LogicalDeviceProxy DeviceContext::createLogicalDevice() {
        auto physicalDevice = this->_physicalDevice.value();

        auto defaultQueuePriorities = {1.0f};
        auto uniqueQueueFamilies = std::set<std::uint32_t>{
                physicalDevice.graphicsQueueFamilyIdx,
                physicalDevice.presentQueueFamilyIdx
        };

        auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>(uniqueQueueFamilies.size());
        std::transform(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end(), queueCreateInfos.begin(),
                       [&defaultQueuePriorities](const std::uint32_t &queueFamilyIdx) {
                           return vk::DeviceQueueCreateInfo()
                                   .setQueueFamilyIndex(queueFamilyIdx)
                                   .setQueuePriorities(defaultQueuePriorities);
                       });

        auto enabledFeatures = vk::PhysicalDeviceFeatures();
        // TODO: enable features for device here

        auto enabledExtensions = std::vector<const char *>(REQUIRED_DEVICE_EXTENSIONS.size());
        std::transform(REQUIRED_DEVICE_EXTENSIONS.begin(), REQUIRED_DEVICE_EXTENSIONS.end(), enabledExtensions.begin(),
                       [](const std::string_view &extension) {
                           return extension.data();
                       });

        auto createInfo = vk::DeviceCreateInfo()
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&enabledFeatures)
                .setPEnabledExtensionNames(enabledExtensions);

        auto device = physicalDevice.handle.createDevice(createInfo);

        return {
                .handle = device,
                .graphicsQueue = device.getQueue(physicalDevice.graphicsQueueFamilyIdx, 0),
                .presentQueue = device.getQueue(physicalDevice.presentQueueFamilyIdx, 0)
        };
    }

    vk::CommandPool DeviceContext::createCommandPool() {
        auto createInfo = vk::CommandPoolCreateInfo()
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(this->_physicalDevice->graphicsQueueFamilyIdx);

        return this->_logicalDevice.value().handle.createCommandPool(createInfo);
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

        return this->_logicalDevice.value().handle.createDescriptorPool(descriptorPoolCreateInfo);
    }

    DeviceContext::DeviceContext(ResourceSet *resources)
            : _vulkanBackend(resources->getLazy<VulkanBackend>()),
              _vkSurfaceProvider(resources->getLazy<VkSurfaceProvider>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()) {
        //
    }

    void DeviceContext::init() {
        try {
            this->_physicalDevice = this->selectPhysicalDevice();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to select physical device"));
        }

        try {
            this->_logicalDevice = this->createLogicalDevice();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create logical device"));
        }

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
            this->_logicalDevice.value().handle.destroy(*this->_descriptorPool);
            this->_descriptorPool = std::nullopt;
        }

        if (this->_commandPool.has_value()) {
            this->_logicalDevice.value().handle.destroy(*this->_commandPool);
            this->_commandPool = std::nullopt;
        }

        if (this->_logicalDevice.has_value()) {
            this->_logicalDevice.value().handle.destroy();
            this->_logicalDevice = std::nullopt;
        }

        this->_physicalDevice = std::nullopt;
    }
}
