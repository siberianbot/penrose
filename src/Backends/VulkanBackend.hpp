#ifndef PENROSE_BACKENDS_VULKAN_BACKEND_HPP
#define PENROSE_BACKENDS_VULKAN_BACKEND_HPP

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Core/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class GlfwBackend;

    class VulkanBackend : public Resource {
    private:
        std::shared_ptr<GlfwBackend> _glfwBackend;

        std::optional<vk::Instance> _instance;

    public:
        explicit VulkanBackend(ResourceSet *resources);
        ~VulkanBackend() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::Instance &getInstance() { return this->_instance.value(); }
    };
}

#endif // PENROSE_BACKENDS_VULKAN_BACKEND_HPP
