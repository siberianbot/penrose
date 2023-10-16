#include "VulkanBackend.hpp"

#include <array>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/CollectionUtils.hpp>

namespace Penrose {

    constexpr static const std::string_view VULKAN_BACKEND_TAG = "VulkanBackend";

    constexpr static const std::array<std::string_view, 0> REQUIRED_LAYERS = {
            // nothing
    };

    constexpr static const std::array<std::string_view, 1> OPTIONAL_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
    };

    constexpr static const std::array<std::string_view, 1> OPTIONAL_EXTENSIONS = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    VulkanBackend::VulkanBackend(ResourceSet *resources)
            : _log(resources->getLazy<Log>()),
              _instanceExtensionsProvider(resources->getLazy<VkInstanceExtensionsProvider>()) {
        //
    }

    void VulkanBackend::init() {

        std::vector<const char *> enabledLayers;
        std::vector<const char *> enabledExtensions;

        {
            auto availableLayers = vk::enumerateInstanceLayerProperties();

            this->_log->writeInfo(VULKAN_BACKEND_TAG, "{} layer(s) available", availableLayers.size());

            for (const auto &layer: availableLayers) {
                this->_log->writeInfo(VULKAN_BACKEND_TAG, "\t{} {}: {}",
                                      static_cast<const char *>(layer.layerName), layer.implementationVersion,
                                      static_cast<const char *>(layer.description));
            }

            for (const auto &requiredLayer: REQUIRED_LAYERS) {

                auto haveLayer = contains(availableLayers, requiredLayer, [](const vk::LayerProperties &layer) {
                    return layer.layerName;
                });

                if (!haveLayer) {
                    throw EngineError("Required Vulkan layer {} is not available", requiredLayer);
                }

                enabledLayers.push_back(requiredLayer.data());
            }

            for (const auto &optionalLayer: OPTIONAL_LAYERS) {

                auto haveLayer = contains(availableLayers, optionalLayer, [](const vk::LayerProperties &layer) {
                    return layer.layerName;
                });

                if (!haveLayer) {
                    this->_log->writeWarning(VULKAN_BACKEND_TAG, "Optional Vulkan layer {} is not available",
                                             optionalLayer);

                    continue;
                }

                enabledLayers.push_back(optionalLayer.data());
            }
        }

        {
            auto availableExtensions = vk::enumerateInstanceExtensionProperties();

            this->_log->writeInfo(VULKAN_BACKEND_TAG, "{} extension(s) available", availableExtensions.size());

            for (const auto &extension: availableExtensions) {
                this->_log->writeInfo(VULKAN_BACKEND_TAG, "\t{}", static_cast<const char *>(extension.extensionName));
            }

            auto requiredExtensions = this->_instanceExtensionsProvider->getRequiredInstanceExtensions();

            for (const auto &requiredExtension: requiredExtensions) {

                auto haveExtension = contains(availableExtensions, requiredExtension,
                                              [](const vk::ExtensionProperties &extension) {
                                                  return extension.extensionName;
                                              });

                if (!haveExtension) {
                    throw EngineError("Required instance extension {} is not available", requiredExtension);
                }

                enabledExtensions.push_back(requiredExtension.data());
            }

            for (const auto &optionalExtension: OPTIONAL_EXTENSIONS) {

                auto haveExtension = contains(availableExtensions, optionalExtension,
                                              [](const vk::ExtensionProperties &extension) {
                                                  return extension.extensionName;
                                              });

                if (!haveExtension) {
                    this->_log->writeWarning(VULKAN_BACKEND_TAG, "Optional instance extension {} is not available",
                                             optionalExtension);

                    continue;
                }

                enabledExtensions.push_back(optionalExtension.data());
            }
        }

        auto debugMessengerAvailable = contains(enabledExtensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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
                .setApiVersion(VK_API_VERSION_1_3)
                .setPEngineName("Penrose")
                .setEngineVersion(VK_MAKE_VERSION(0, 1, 0))
                .setPApplicationName("Penrose")
                .setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));

        auto createInfo = vk::InstanceCreateInfo()
                .setPNext(debugMessengerAvailable ? &debugMessenger : nullptr)
                .setPApplicationInfo(&applicationInfo)
                .setPEnabledExtensionNames(enabledExtensions)
                .setPEnabledLayerNames(enabledLayers);

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
