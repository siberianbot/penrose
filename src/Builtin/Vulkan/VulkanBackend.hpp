#ifndef PENROSE_BUILTIN_VULKAN_VULKAN_BACKEND_HPP
#define PENROSE_BUILTIN_VULKAN_VULKAN_BACKEND_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Vulkan/VkInstanceExtensionsProvider.hpp>

namespace Penrose {

    class VulkanBackend final: public Resource<VulkanBackend>,
                               public Initializable {
    public:
        explicit VulkanBackend(const ResourceSet *resources);
        ~VulkanBackend() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::Instance &getInstance() { return this->_instance.value(); }

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<VkInstanceExtensionsProvider> _instanceExtensionsProvider;

        std::optional<vk::Instance> _instance;

        static VkBool32 debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData
        );
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VULKAN_BACKEND_HPP
