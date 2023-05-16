#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Core/Resource.hpp"

struct GLFWwindow;

namespace Penrose {

    class ResourceSet;
    class EventQueue;
    class VulkanBackend;

    class Surface : public Resource {
    private:
        std::shared_ptr<EventQueue> _eventQueue;
        std::shared_ptr<VulkanBackend> _vulkanBackend;

        GLFWwindow *_handle = nullptr;
        std::optional<vk::SurfaceKHR> _surface = std::nullopt;

        static void closeCallback(GLFWwindow *handle);

    public:
        explicit Surface(ResourceSet *resources);
        ~Surface() override = default;

        void init() override;
        void destroy() override;

        void poll();

        [[nodiscard]] vk::SurfaceKHR &getSurface() { return this->_surface.value(); }
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
