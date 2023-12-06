#ifndef PENROSE_RENDERING_RENDER_MANAGER_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_HPP

#include <thread>

#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Resources/Runnable.hpp>

namespace Penrose {

    class ResourceSet;

    class RenderManager : public Resource<RenderManager>,
                          public Initializable,
                          public Runnable {
    public:
        explicit RenderManager(const ResourceSet *resources);
        ~RenderManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override { /* nothing to do */ }

        void run() override;
        void stop() override;

    private:
        ResourceProxy<RenderSystem> _renderSystem;

        std::optional<std::jthread> _thread;
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_HPP
