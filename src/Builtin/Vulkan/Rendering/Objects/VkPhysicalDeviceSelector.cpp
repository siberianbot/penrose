#include "VkPhysicalDeviceSelector.hpp"

#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/CollectionUtils.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkPhysicalDeviceSelector";

    static constexpr std::array DEVICE_PRIORITY = {
        vk::PhysicalDeviceType::eDiscreteGpu, vk::PhysicalDeviceType::eIntegratedGpu,
        vk::PhysicalDeviceType::eVirtualGpu,  vk::PhysicalDeviceType::eCpu,
        vk::PhysicalDeviceType::eOther,
    };

    VkPhysicalDeviceSelector::VkPhysicalDeviceSelector(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _surfaceManager(resources->get<SurfaceManager>()),
          _vkSurfaceProvider(resources->get<VkSurfaceProvider>()),
          _vulkanBackend(resources->get<VulkanBackend>()) {
        //
    }

    VkPhysicalDevice VkPhysicalDeviceSelector::selectPhysicalDevice() {
        const auto devices = this->_vulkanBackend->getInstance().enumeratePhysicalDevices();
        const auto surface = this->_vkSurfaceProvider->getVkSurfaceFor(this->_surfaceManager->getSurface().get());

        std::vector<VkPhysicalDevice> availableDevices;

        for (const auto &device: devices) {
            auto deviceInfo = this->makePhysicalDevice(device, surface);

            if (!deviceInfo.has_value()) {
                continue;
            }

            availableDevices.push_back(*deviceInfo);
        }

        if (availableDevices.empty()) {
            throw EngineError("No suitable physical device available");
        }

        std::ranges::sort(availableDevices, [](const VkPhysicalDevice &a, const VkPhysicalDevice &b) {
            return a.priority < b.priority;
        });

        this->_log->writeInfo(TAG, "{} out of {} device(s) available", devices.size(), availableDevices.size());

        for (const auto &deviceInfo: availableDevices) {
            this->_log->writeInfo(
                TAG, "\tPriority {}: {}", deviceInfo.priority,
                static_cast<const char *>(deviceInfo.properties.deviceName)
            );
        }

        return availableDevices.at(0);
    }

    std::optional<VkPhysicalDevice> VkPhysicalDeviceSelector::makePhysicalDevice(
        const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface
    ) {
        auto deviceInfo = VkPhysicalDevice {
            .handle = device,
            .features = device.getFeatures(),
            .properties = device.getProperties(),
        };

        const auto queueFamilyProperties = device.getQueueFamilyProperties();

        std::optional<std::uint32_t> graphicsIdx;
        std::optional<std::uint32_t> transferIdx;
        std::optional<std::uint32_t> presentIdx;

        for (std::uint32_t idx = 0; idx < queueFamilyProperties.size(); ++idx) {
            if (!graphicsIdx.has_value() && queueFamilyProperties[idx].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsIdx = idx;
            }

            if (!transferIdx.has_value() && queueFamilyProperties[idx].queueFlags & vk::QueueFlagBits::eTransfer) {
                transferIdx = idx;
            }

            if (!presentIdx.has_value() && device.getSurfaceSupportKHR(idx, surface)) {
                presentIdx = idx;
            }

            if (graphicsIdx.has_value() && transferIdx.has_value() && presentIdx.has_value()) {
                break;
            }
        }

        if (!graphicsIdx.has_value() || !transferIdx.has_value() || !presentIdx.has_value()) {
            this->_log->writeDebug(
                TAG, "Device {} discarded: some of the family queues are not supported",
                static_cast<const char *>(deviceInfo.properties.deviceName)
            );

            return std::nullopt;
        }

        deviceInfo.graphicsFamilyIdx = *graphicsIdx;
        deviceInfo.transferFamilyIdx = *transferIdx;
        deviceInfo.presentFamilyIdx = *presentIdx;

        auto extensions = device.enumerateDeviceExtensionProperties();
        std::ranges::transform(
            extensions, std::back_inserter(deviceInfo.extensions),
            [](const vk::ExtensionProperties &extension) { return extension.extensionName; }
        );

        for (const auto &requiredDeviceExtension: REQUIRED_DEVICE_EXTENSIONS) {
            const auto haveExtension = contains(
                extensions, requiredDeviceExtension,
                [](const vk::ExtensionProperties &extension) {
                    return std::string_view(extension.extensionName, std::strlen(extension.extensionName));
                }
            );

            if (haveExtension) {
                continue;
            }

            this->_log->writeDebug(
                TAG, "Device {} discarded: required extension {} is not supported",
                static_cast<const char *>(deviceInfo.properties.deviceName), requiredDeviceExtension
            );

            return std::nullopt;
        }

        for (std::uint32_t priorityIdx = 0; priorityIdx < DEVICE_PRIORITY.size(); ++priorityIdx) {
            if (deviceInfo.properties.deviceType == DEVICE_PRIORITY.at(priorityIdx)) {
                deviceInfo.priority = priorityIdx;
                break;
            }
        }

        return deviceInfo;
    }
}
