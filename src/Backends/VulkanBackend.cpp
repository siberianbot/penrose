#include "VulkanBackend.hpp"

#include "src/Backends/GlfwBackend.hpp"
#include "src/Common/EngineError.hpp"
#include "src/Core/ResourceSet.hpp"

namespace Penrose {

    VulkanBackend::VulkanBackend(ResourceSet *resources)
            : _glfwBackend(resources->get<GlfwBackend>()->lock()) {
        //
    }

    void VulkanBackend::init() {
        auto requiredExtensions = this->_glfwBackend->getRequiredInstanceExtensions();
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        auto requiredLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        auto applicationInfo = vk::ApplicationInfo()
                .setApiVersion(VK_VERSION_1_3)
                .setPEngineName("Penrose")
                .setEngineVersion(VK_MAKE_VERSION(0, 1, 0))
                .setPApplicationName("Penrose")
                .setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));

        auto createInfo = vk::InstanceCreateInfo()
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
