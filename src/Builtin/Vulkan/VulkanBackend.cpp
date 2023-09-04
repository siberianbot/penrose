#include "VulkanBackend.hpp"

#include <array>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr static const std::string_view VULKAN_BACKEND_TAG = "VulkanBackend";

    constexpr static const std::array<const char *, 1> REQUIRED_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
    };

    VulkanBackend::VulkanBackend(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _instanceExtensionsProvider(resources->getLazy<VkInstanceExtensionsProvider>()) {
        //
    }

    void VulkanBackend::init() {
        auto requiredExtensions = this->_instanceExtensionsProvider->getRequiredInstanceExtensions();
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        auto debugMessenger = vk::DebugUtilsMessengerCreateInfoEXT()
                .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding |
                                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
                .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                .setPfnUserCallback(debugCallback)
                .setPUserData(this);

        auto applicationInfo = vk::ApplicationInfo()
                .setApiVersion(VK_VERSION_1_3)
                .setPEngineName("Penrose")
                .setEngineVersion(VK_MAKE_VERSION(0, 1, 0))
                .setPApplicationName("Penrose")
                .setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));

        auto createInfo = vk::InstanceCreateInfo()
                .setPNext(&debugMessenger)
                .setPApplicationInfo(&applicationInfo)
                .setPEnabledExtensionNames(requiredExtensions)
                .setPEnabledLayerNames(REQUIRED_LAYERS);

        try {
            this->_instance = vk::createInstance(createInfo);
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to initialize Vulkan instance"));
        }
    }

    void VulkanBackend::destroy() {
        if (!this->_instance.has_value()) {
            return;
        }

        this->_instance->destroy();
        this->_instance = std::nullopt;
    }

    VkBool32 VulkanBackend::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                          VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                          void *pUserData) {
        auto that = reinterpret_cast<VulkanBackend *>(pUserData);

        switch (messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                that->_log->writeDebug(VULKAN_BACKEND_TAG, pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                that->_log->writeInfo(VULKAN_BACKEND_TAG, pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                that->_log->writeWarning(VULKAN_BACKEND_TAG, pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                that->_log->writeError(VULKAN_BACKEND_TAG, pCallbackData->pMessage);
                break;

            default:
                throw EngineError("Message severity is not supported");
        }

        return false;
    }
}
