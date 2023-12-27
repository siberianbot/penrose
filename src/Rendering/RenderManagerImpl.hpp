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
        const ResourceSet *_resources;
        ResourceProxy<Log> _log;
        ResourceProxy<SurfaceEventQueue> _surfaceEventQueue;

        JobQueue _jobQueue;

        std::optional<RenderSystem *> _renderSystem;
        std::map<std::string, Renderer *> _renderers;
        std::optional<std::unique_ptr<RenderContext>> _renderContext;

        RenderExecutionInfo _executionInfo;

        void render();
        void invalidate();
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_IMPL_HPP
