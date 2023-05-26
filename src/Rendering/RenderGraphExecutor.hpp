#ifndef PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP

#include <array>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "src/Constants.hpp"
#include "src/Common/Initializable.hpp"
#include "src/Events/Event.hpp"
#include "src/Rendering/RenderGraph.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;
    class DeviceContext;
    class DeviceMemoryAllocator;
    class PresentContext;
    class RenderContext;

    class RenderGraphExecutor : public Resource, public Initializable {
    private:
        class Target {
        public:
            virtual ~Target() = default;

            [[nodiscard]] virtual const vk::ImageView &getView(const std::uint32_t &imageIdx) const = 0;

            [[nodiscard]] virtual const vk::Extent2D &getExtent() const = 0;
        };

        class SwapchainTarget : public Target {
        private:
            PresentContext *_presentContext;

        public:
            explicit SwapchainTarget(PresentContext *presentContext);
            ~SwapchainTarget() override = default;

            [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &imageIdx) const override;

            [[nodiscard]] const vk::Extent2D &getExtent() const override;
        };

        class ImageTarget : public Target {
        private:
            DeviceContext *_deviceContext;
            DeviceMemoryAllocator *_deviceMemoryAllocator;
            vk::Image _image;
            vk::ImageView _imageView;
            vk::Extent2D _extent;

        public:
            ImageTarget(DeviceContext *deviceContext,
                        DeviceMemoryAllocator *deviceMemoryAllocator,
                        vk::Image image,
                        vk::ImageView imageView,
                        vk::Extent2D extent);
            ~ImageTarget() override;

            [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &) const override {
                return this->_imageView;
            }

            [[nodiscard]] const vk::Extent2D &getExtent() const override {
                return this->_extent;
            }
        };

        class Subpass {
            // TODO
        };

        class Pass {
        private:
            DeviceContext *_deviceContext;
            vk::RenderPass _renderPass;
            std::vector<vk::ClearValue> _clearValues;
            std::vector<std::uint32_t> _targets;
            std::vector<Subpass> _subpasses;

        public:
            Pass(DeviceContext *deviceContext,
                 vk::RenderPass renderPass,
                 std::vector<vk::ClearValue> clearValues,
                 std::vector<std::uint32_t> targets,
                 std::vector<Subpass> subpasses);
            ~Pass();

            [[nodiscard]] const vk::RenderPass &getRenderPass() const {
                return this->_renderPass;
            }

            [[nodiscard]] const std::vector<vk::ClearValue> &getClearValues() const {
                return this->_clearValues;
            }

            [[nodiscard]] const std::vector<std::uint32_t> &getTargets() const {
                return this->_targets;
            }

            [[nodiscard]] const std::vector<Subpass> &getSubpasses() const {
                return this->_subpasses;
            }
        };

        class Framebuffer {
        private:
            DeviceContext *_deviceContext;
            std::vector<vk::Framebuffer> _framebuffers;

        public:
            Framebuffer(DeviceContext *deviceContext,
                        std::vector<vk::Framebuffer> framebuffers);
            ~Framebuffer();

            [[nodiscard]] const vk::Framebuffer &getFramebuffer(const std::uint32_t &imageIdx) const {
                return this->_framebuffers.at(imageIdx);
            }
        };

        struct GraphState {
            RenderGraph graph;
            std::vector<std::unique_ptr<Target>> targets;
            std::vector<std::unique_ptr<Pass>> passes;
        };

        struct FramebufferState {
            std::vector<std::unique_ptr<Framebuffer>> framebuffers;
        };

        EventQueue *_eventQueue;
        DeviceContext *_deviceContext;
        DeviceMemoryAllocator *_deviceMemoryAllocator;
        PresentContext *_presentContext;
        RenderContext *_renderContext;

        std::optional<vk::CommandPool> _commandPool;
        std::array<vk::CommandBuffer, INFLIGHT_FRAME_COUNT> _commandBuffers;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _graphExecutedSemaphores;

        std::mutex _executorMutex;
        std::optional<EventHandlerIndex> _eventHandlerIdx;
        std::optional<GraphState> _graphState;
        std::optional<FramebufferState> _framebufferState;

        std::unique_ptr<Target> createTarget(const RenderTarget &target);
        std::unique_ptr<Pass> createPass(const RenderSubgraph &subgraph);
        std::unique_ptr<Framebuffer> createFramebuffer(const GraphState &graphState,
                                                       const std::unique_ptr<Pass> &pass);

        GraphState createGraphState(const RenderGraph &graph);
        FramebufferState createFramebufferState(const GraphState &graphState);

        void cleanup(bool framebuffersOnly);

    public:
        explicit RenderGraphExecutor(ResourceSet *resources);
        ~RenderGraphExecutor() override = default;

        void init() override;
        void destroy() override;

        void execute(const std::uint32_t &frameIdx,
                     const std::uint32_t &imageIdx,
                     const vk::Fence &fence);

        void invalidate();

        [[nodiscard]] std::lock_guard<std::mutex> acquireExecutorLock() {
            return std::lock_guard<std::mutex>(this->_executorMutex);
        }

        [[nodiscard]] const std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> &getImageReadySemaphores() const {
            return this->_imageReadySemaphores;
        }

        [[nodiscard]] const std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> &getGraphExecutedSemaphores() const {
            return this->_graphExecutedSemaphores;
        }
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_HPP