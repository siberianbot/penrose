#ifndef PENROSE_RENDERING_RENDER_THREAD_HPP
#define PENROSE_RENDERING_RENDER_THREAD_HPP

#include <memory>
#include <optional>
#include <thread>

#include <vulkan/vulkan.hpp>

#include "src/Core/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;

    class RenderThread : public Resource {
    private:
        struct InflightFrameSync {
            vk::Fence fence;
            vk::Semaphore imageAvailable, renderFinished;
        };

        std::shared_ptr<DeviceContext> _deviceContext;
        std::shared_ptr<PresentContext> _presentContext;

        // TODO: remove, only for rendering stub
        vk::CommandPool _commandPool;
        std::vector<vk::CommandBuffer> _commandBuffers;

        std::vector<InflightFrameSync> _syncs;
        std::optional<std::jthread> _thread;
        std::uint32_t _currentFrameIdx = 0;

        bool renderFrame(const std::uint32_t &frameIdx);

    public:
        explicit RenderThread(ResourceSet *resources);
        ~RenderThread() override = default;

        void init() override;
        void destroy() override;
    };
}

#endif // PENROSE_RENDERING_RENDER_THREAD_HPP
