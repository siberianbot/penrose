#ifndef PENROSE_BACKENDS_VULKAN_BACKEND_HPP
#define PENROSE_BACKENDS_VULKAN_BACKEND_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class GlfwBackend;

    class VulkanBackend : public Resource, public Initializable {
    private:
        GlfwBackend *_glfwBackend;

        std::optional<vk::Instance> _instance;

        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                      void *pUserData);

    public:
        explicit VulkanBackend(ResourceSet *resources);
        ~VulkanBackend() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::Instance &getInstance() { return this->_instance.value(); }
    };
}

#endif // PENROSE_BACKENDS_VULKAN_BACKEND_HPP
