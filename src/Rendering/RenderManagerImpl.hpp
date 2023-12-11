#ifndef PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP

#include <map>
#include <optional>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Common/JobQueue.hpp"

namespace Penrose {

    class RenderManagerImpl final: public Resource<RenderManagerImpl>,
                                   public Initializable,
                                   public RenderManager {
    public:
        explicit RenderManagerImpl(const ResourceSet *resources);
        ~RenderManagerImpl() override = default;

        void init() override;
        void destroy() override;

        void setRenderSystem(std::type_index &&type) override;

        void addRenderer(std::type_index &&type) override;

    private:
        enum class RenderJobType : int {
            SurfaceResize = -1,
            FrameRender = 0
        };

        class FrameRenderJob final: public Job {
        public:
            explicit FrameRenderJob(RenderSystem *renderSystem);
            ~FrameRenderJob() override = default;

            [[nodiscard]] int order() const override { return static_cast<int>(RenderJobType::FrameRender); }

            [[nodiscard]] bool remove() const override { return false; }

            void exec() override;

        private:
            RenderSystem *_renderSystem;
        };

        class SurfaceResizeJob final: public Job {
        public:
            explicit SurfaceResizeJob(RenderSystem *renderSystem);
            ~SurfaceResizeJob() override = default;

            [[nodiscard]] int order() const override { return static_cast<int>(RenderJobType::SurfaceResize); }

            [[nodiscard]] bool remove() const override { return true; }

            void exec() override;

        private:
            RenderSystem *_renderSystem;
        };

        const ResourceSet *_resources;
        ResourceProxy<Log> _log;

        JobQueue _jobQueue;

        std::optional<RenderSystem *> _renderSystem;
        std::map<std::type_index, Renderer *> _renderers;
        bool _initialized;
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP
