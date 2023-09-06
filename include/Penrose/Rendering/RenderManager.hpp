#ifndef PENROSE_RENDERING_RENDER_MANAGER_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_HPP

#include <thread>

#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Runnable.hpp>

namespace Penrose {

    class ResourceSet;

    class RenderManager : public Resource, public Runnable {
    public:
        explicit RenderManager(ResourceSet *resources);
        ~RenderManager() override = default;

        void run() override;
        void stop() override;

    private:
        Lazy<RenderSystem> _renderSystem;

        std::optional<std::jthread> _thread;
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_HPP
