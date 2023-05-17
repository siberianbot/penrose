#include "DeviceContext.hpp"

#include <array>
#include <set>
#include <string_view>

#include "src/Backends/VulkanBackend.hpp"
#include "src/Common/EngineError.hpp"
#include "src/Core/ResourceSet.hpp"
#include "src/Rendering/Surface.hpp"

namespace Penrose {

    static constexpr const std::array<std::string_view, 1> REQUIRED_DEVICE_EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    DeviceContext::PhysicalDeviceProxy DeviceContext::selectPhysicalDevice() {
        auto devices = this->_vulkanBackend->getInstance().enumeratePhysicalDevices();

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

                if (device.getSurfaceSupportKHR(idx, this->_surface->getSurface())) {
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

    DeviceContext::DeviceContext(ResourceSet *resources)
            : _vulkanBackend(resources->get<VulkanBackend>()->lock()),
              _surface(resources->get<Surface>()->lock()) {
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
    }

    void DeviceContext::destroy() {
        if (this->_logicalDevice.has_value()) {
            this->_logicalDevice.value().handle.destroy();
            this->_logicalDevice = std::nullopt;
        }

        this->_physicalDevice = std::nullopt;
    }
}
