#ifndef PENROSE_RENDERING_RENDER_THREAD_HPP
#define PENROSE_RENDERING_RENDER_THREAD_HPP

#include <optional>
#include <thread>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;

    class RenderThread : public Resource, public Initializable {
    private:
        struct InflightFrameSync {
            vk::Fence fence;
            vk::Semaphore imageAvailable, renderFinished;
        };

        DeviceContext *_deviceContext;
        PresentContext *_presentContext;

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
