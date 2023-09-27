#include "VkPhysicalDeviceContext.hpp"

#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    constexpr static const std::string_view VK_PHYSICAL_DEVICE_CONTEXT_TAG = "VkPhysicalDeviceContext";

    VkPhysicalDeviceContext::VkPhysicalDeviceContext(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()),
              _vkSurfaceProvider(resources->getLazy<VkSurfaceProvider>()),
              _vulkanBackend(resources->getLazy<VulkanBackend>()) {
        //
    }

    void VkPhysicalDeviceContext::init() {
        this->_log->writeInfo(VK_PHYSICAL_DEVICE_CONTEXT_TAG, "Choosing suitable physical device");

        try {
            this->_state = this->selectPhysicalDevice();
        } catch (const std::exception &) {
            std::throw_with_nested(EngineError("Failed to select physical device"));
        }

        this->_log->writeInfo(VK_PHYSICAL_DEVICE_CONTEXT_TAG,
                              "Chosen device: device id = {:#x}, device name = {}, driver version = {:#x}",
                              this->_state->properties.deviceID,
                              static_cast<char *>(this->_state->properties.deviceName),
                              this->_state->properties.driverVersion);
    }

    void VkPhysicalDeviceContext::destroy() {
        this->_state = std::nullopt;
    }

    VkPhysicalDeviceContext::State VkPhysicalDeviceContext::selectPhysicalDevice() {
        auto devices = this->_vulkanBackend->getInstance().enumeratePhysicalDevices();
        auto surface = this->_vkSurfaceProvider->getVkSurfaceFor(this->_surfaceManager->getSurface().get());

        for (const auto &device: devices) {
            auto properties = device.getProperties();

            this->_log->writeDebug(VK_PHYSICAL_DEVICE_CONTEXT_TAG,
                                   "Checking device {:#x} {} for suitability",
                                   properties.deviceID,
                                   static_cast<const char *>(properties.deviceName));

            auto queueFamilyProperties = device.getQueueFamilyProperties();

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
                this->_log->writeDebug(VK_PHYSICAL_DEVICE_CONTEXT_TAG,
                                       "Device {} discarded: some of the family queue are not supported",
                                       static_cast<const char *>(properties.deviceName));

                continue;
            }

            auto extensionProperties = device.enumerateDeviceExtensionProperties();
            auto missingExtensions = std::vector<const char *>(REQUIRED_DEVICE_EXTENSIONS.begin(),
                                                               REQUIRED_DEVICE_EXTENSIONS.end());

            for (const auto &extension: extensionProperties) {
                missingExtensions.erase(
                        std::remove_if(missingExtensions.begin(), missingExtensions.end(),
                                       [&extension](const char *e) { return strcmp(e, extension.extensionName) == 0; }),
                        missingExtensions.end()
                );
            }

            if (!missingExtensions.empty()) {
                this->_log->writeDebug(VK_PHYSICAL_DEVICE_CONTEXT_TAG,
                                       "Device {} discarded: some of the required extensions are not supported",
                                       static_cast<const char *>(properties.deviceName));

                continue;
            }

            return {
                    .handle = device,
                    .features = device.getFeatures(),
                    .properties = properties,
                    .graphicsFamilyIdx = graphicsIdx.value(),
                    .transferFamilyIdx = transferIdx.value(),
                    .presentFamilyIdx = presentIdx.value()
            };
        }

        throw EngineError("No suitable physical device available");
    }
}
