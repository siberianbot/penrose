#ifndef PENROSE_RENDERING_RENDER_THREAD_HPP
#define PENROSE_RENDERING_RENDER_THREAD_HPP

#include <array>
#include <optional>
#include <thread>

#include <vulkan/vulkan.hpp>

#include "src/Constants.hpp"
#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;
    class RenderGraphExecutor;

    class RenderThread : public Resource, public Initializable {
    private:
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
        RenderGraphExecutor *_renderGraphExecutor;

        std::array<vk::Fence, INFLIGHT_FRAME_COUNT> _fences;
        std::uint32_t _currentFrameIdx = 0;

        std::optional<std::jthread> _thread;

        bool renderFrame(const std::uint32_t &frameIdx);

    public:
        explicit RenderThread(ResourceSet *resources);
        ~RenderThread() override = default;

        void init() override;
        void destroy() override;
    };
}

#endif // PENROSE_RENDERING_RENDER_THREAD_HPP
