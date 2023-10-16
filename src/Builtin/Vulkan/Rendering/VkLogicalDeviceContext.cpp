#include "VkLogicalDeviceContext.hpp"

#include <algorithm>
#include <cstdint>
#include <set>
#include <vector>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    constexpr static const std::string_view VK_LOGICAL_DEVICE_CONTEXT_TAG = "VkLogicalDeviceContext";

    VkLogicalDeviceContext::VkLogicalDeviceContext(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _physicalDeviceContext(resources->getLazy<VkPhysicalDeviceContext>()) {
        //
    }

    void VkLogicalDeviceContext::init() {
        this->_log->writeInfo(VK_LOGICAL_DEVICE_CONTEXT_TAG, "Creating logical device");

        try {
            this->_state = this->createLogicalDevice();
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to create logical device"));
        }
    }

    void VkLogicalDeviceContext::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        this->_state->handle.destroy();
        this->_state = std::nullopt;
    }

    VkLogicalDeviceContext::State VkLogicalDeviceContext::createLogicalDevice() {
        auto defaultQueuePriorities = {1.0f};
        auto uniqueQueueFamilies = std::set<std::uint32_t>{
                this->_physicalDeviceContext->getGraphicsFamilyIdx(),
                this->_physicalDeviceContext->getTransferFamilyIdx(),
                this->_physicalDeviceContext->getPresentFamilyIdx()
        };

        auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>(uniqueQueueFamilies.size());
        std::transform(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end(), queueCreateInfos.begin(),
                       [&defaultQueuePriorities](const std::uint32_t &familyIdx) {
                           return vk::DeviceQueueCreateInfo()
                                   .setQueueFamilyIndex(familyIdx)
                                   .setQueuePriorities(defaultQueuePriorities);
                       });

        auto enabledFeatures = vk::PhysicalDeviceFeatures();

        auto enabledExtensions = std::vector<const char *>(REQUIRED_DEVICE_EXTENSIONS.size());
        std::transform(REQUIRED_DEVICE_EXTENSIONS.begin(), REQUIRED_DEVICE_EXTENSIONS.end(), enabledExtensions.begin(),
                       [](const std::string_view &extension) {
                           return extension.data();
                       });

        auto createInfo = vk::DeviceCreateInfo()
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&enabledFeatures)
                .setPEnabledExtensionNames(enabledExtensions);

        auto device = this->_physicalDeviceContext->getHandle().createDevice(createInfo);

        return {
                .handle = device,
                .graphicsQueue = device.getQueue(this->_physicalDeviceContext->getGraphicsFamilyIdx(), 0),
                .transferQueue = device.getQueue(this->_physicalDeviceContext->getTransferFamilyIdx(), 0),
                .presentQueue = device.getQueue(this->_physicalDeviceContext->getPresentFamilyIdx(), 0)
        };
    }
}
