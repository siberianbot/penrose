#ifndef PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP
#define PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class GlfwBackend;

    class VulkanBackend : public InitializableResource {
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

#endif // PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP
