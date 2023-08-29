#ifndef PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP
#define PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class GlfwBackend;
    class Log;

    class VulkanBackend : public Resource, public Initializable {
    public:
        explicit VulkanBackend(ResourceSet *resources);
        ~VulkanBackend() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::Instance &getInstance() { return this->_instance.value(); }

    private:
        GlfwBackend *_glfwBackend;
        Log *_log;

        std::optional<vk::Instance> _instance;

        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                      void *pUserData);
    };
}

#endif // PENROSE_BUILTIN_BACKENDS_VULKAN_BACKEND_HPP
