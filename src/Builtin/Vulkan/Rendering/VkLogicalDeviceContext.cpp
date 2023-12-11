#include "VkLogicalDeviceContext.hpp"

#include <algorithm>
#include <cstdint>
#include <set>
#include <string_view>
#include <vector>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    inline constexpr std::string_view TAG = "VkLogicalDeviceContext";

    VkLogicalDeviceContext::VkLogicalDeviceContext(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _physicalDeviceContext(resources->get<VkPhysicalDeviceContext>()) {
        //
    }

    void VkLogicalDeviceContext::init() {
        this->_log->writeInfo(TAG, "Creating logical device");

        try {
            this->_state = this->makeLogicalDevice();
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

    VkLogicalDeviceContext::LogicalDeviceState VkLogicalDeviceContext::makeLogicalDevice() {
        auto defaultQueuePriorities = {1.0f};
        auto uniqueQueueFamilies = std::set {
            this->_physicalDeviceContext->getGraphicsFamilyIdx(),
            this->_physicalDeviceContext->getTransferFamilyIdx(),
            this->_physicalDeviceContext->getPresentFamilyIdx(),
        };

        auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>(uniqueQueueFamilies.size());
        std::ranges::transform(
            uniqueQueueFamilies, queueCreateInfos.begin(),
            [&defaultQueuePriorities](const std::uint32_t &familyIdx) {
                return vk::DeviceQueueCreateInfo().setQueueFamilyIndex(familyIdx).setQueuePriorities(
                    defaultQueuePriorities
                );
            }
        );

        // TODO: enabled features should be specified here
        constexpr auto enabledFeatures = vk::PhysicalDeviceFeatures();

        auto enabledExtensions = std::vector<const char *>(REQUIRED_DEVICE_EXTENSIONS.size());
        std::ranges::transform(
            REQUIRED_DEVICE_EXTENSIONS, enabledExtensions.begin(),
            [](const std::string_view &extension) { return extension.data(); }
        );

        const auto createInfo = vk::DeviceCreateInfo()
                                    .setQueueCreateInfos(queueCreateInfos)
                                    .setPEnabledFeatures(&enabledFeatures)
                                    .setPEnabledExtensionNames(enabledExtensions);

        const auto device = this->_physicalDeviceContext->getHandle().createDevice(createInfo);

        return {
            .handle = device,
            .graphicsQueue = device.getQueue(this->_physicalDeviceContext->getGraphicsFamilyIdx(), 0),
            .transferQueue = device.getQueue(this->_physicalDeviceContext->getTransferFamilyIdx(), 0),
            .presentQueue = device.getQueue(this->_physicalDeviceContext->getPresentFamilyIdx(), 0)
        };
    }
}
