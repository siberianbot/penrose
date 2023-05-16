#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include <memory>

#include "src/Core/Resource.hpp"

struct GLFWwindow;

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    class Surface : public Resource {
    private:
        std::shared_ptr<EventQueue> _eventQueue;

        GLFWwindow *_handle = nullptr;

        static void closeCallback(GLFWwindow *handle);

    public:
        explicit Surface(ResourceSet *resources);
        ~Surface() override = default;

        void init() override;
        void destroy() override;

        void poll();
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
