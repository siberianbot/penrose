#include "VulkanBackend.hpp"

#include <iostream>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Backends/GlfwBackend.hpp"

namespace Penrose {

    VkBool32 VulkanBackend::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                          VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                          void *pUserData) {
        auto that = reinterpret_cast<VulkanBackend *>(pUserData);

        // TODO: replace with our logging system
        std::cout << pCallbackData->pMessage << std::endl;

        return false;
    }

    VulkanBackend::VulkanBackend(ResourceSet *resources)
            : _glfwBackend(resources->get<GlfwBackend>()) {
        //
    }

    void VulkanBackend::init() {
        auto requiredExtensions = this->_glfwBackend->getRequiredInstanceExtensions();
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        auto requiredLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

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
                .setPEnabledLayerNames(requiredLayers);

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
}
