#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Common/Types.hpp"
#include "src/Resources/Resource.hpp"

struct GLFWwindow;

namespace Penrose {

    class ResourceSet;
    class EventQueue;
    class VulkanBackend;

    class Surface : public Resource, public Initializable {
    private:
        EventQueue *_eventQueue;
        VulkanBackend *_vulkanBackend;

        GLFWwindow *_handle = nullptr;
        std::optional<vk::SurfaceKHR> _surface;

        static void windowCloseCallback(GLFWwindow *handle);
        static void framebufferSizeCallback(GLFWwindow *handle, int width, int height);

    public:
        explicit Surface(ResourceSet *resources);
        ~Surface() override = default;

        void init() override;
        void destroy() override;

        void poll();

        [[nodiscard]] Size getSize() const;

        [[nodiscard]] vk::SurfaceKHR &getSurface() { return this->_surface.value(); }
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
