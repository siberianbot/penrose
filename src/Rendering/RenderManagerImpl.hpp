#ifndef PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP

#include <map>
#include <memory>
#include <optional>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Events/SurfaceEvents.hpp>
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

        void setExecutionInfo(RenderExecutionInfo &&executionInfo) override { this->_executionInfo = executionInfo; }

        [[nodiscard]] RenderExecutionInfo getExecutionInfo() override { return this->_executionInfo; }

    private:
        enum class RenderJobType : int {
            SurfaceResize = -1,
            FrameRender = 0
        };

        class FrameRenderJob final: public Job {
        public:
            explicit FrameRenderJob(Log *log, RenderManagerImpl *renderManager);
            ~FrameRenderJob() override = default;

            [[nodiscard]] int order() const override { return static_cast<int>(RenderJobType::FrameRender); }

            [[nodiscard]] bool remove() const override { return false; }

            void exec() override;

        private:
            Log *_log;
            RenderManagerImpl *_renderManager;
        };

        class SurfaceResizeJob final: public Job {
        public:
            explicit SurfaceResizeJob(Log *log, RenderContext *renderContext);
            ~SurfaceResizeJob() override = default;

            [[nodiscard]] int order() const override { return static_cast<int>(RenderJobType::SurfaceResize); }

            [[nodiscard]] bool remove() const override { return true; }

            void exec() override;

        private:
            Log *_log;
            RenderContext *_renderContext;
        };

        const ResourceSet *_resources;
        ResourceProxy<Log> _log;
        ResourceProxy<SurfaceEventQueue> _surfaceEventQueue;

        JobQueue _jobQueue;

        std::optional<RenderSystem *> _renderSystem;
        std::map<std::string, Renderer *> _renderers;
        std::optional<std::unique_ptr<RenderContext>> _renderContext;
        bool _initialized;

        RenderExecutionInfo _executionInfo;

        void render();
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP
