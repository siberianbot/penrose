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
#include "src/Rendering/Utils.hpp"
#include "src/Rendering/Operators/RenderOperator.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;
    class DeviceContext;
    class PresentContext;
    class RenderContext;
    class RenderListProvider;

    class RenderGraphExecutor : public Resource, public Initializable {
    private:
        class Target {
        public:
            virtual ~Target() = default;

            [[nodiscard]] virtual const vk::ImageView &getView(const std::uint32_t &imageIdx) const = 0;
        };

        class SwapchainTarget : public Target {
        private:
            PresentContext *_presentContext;

        public:
            explicit SwapchainTarget(PresentContext *presentContext);
            ~SwapchainTarget() override = default;

            [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &imageIdx) const override;
        };

        class ImageTarget : public Target {
        private:
            Image _image;
            DeviceMemory _deviceMemory;
            ImageView _imageView;

        public:
            ImageTarget(Image &&image,
                        DeviceMemory &&deviceMemory,
                        ImageView &&imageView);
            ~ImageTarget() override = default;

            [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &) const override {
                return this->_imageView.getInstance();
            }
        };

        class Pass {
        private:
            DeviceContext *_deviceContext;
            vk::RenderPass _renderPass;
            std::vector<vk::ClearValue> _clearValues;
            std::vector<std::uint32_t> _targets;
            std::vector<std::optional<std::unique_ptr<RenderOperator>>> _operators;
            std::optional<vk::Extent2D> _renderArea;

        public:
            Pass(DeviceContext *deviceContext,
                 vk::RenderPass renderPass,
                 std::vector<vk::ClearValue> clearValues,
                 std::vector<std::uint32_t> targets,
                 std::vector<std::optional<std::unique_ptr<RenderOperator>>> operators,
                 std::optional<vk::Extent2D> renderArea);
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

            [[nodiscard]] const std::vector<std::optional<std::unique_ptr<RenderOperator>>> &getOperators() const {
                return this->_operators;
            }

            [[nodiscard]] const std::optional<vk::Extent2D> &getRenderArea() const {
                return this->_renderArea;
            }
        };

        class Framebuffer {
        private:
            DeviceContext *_deviceContext;
            std::vector<vk::Framebuffer> _framebuffers;
            vk::Extent2D _renderArea;

        public:
            Framebuffer(DeviceContext *deviceContext,
                        std::vector<vk::Framebuffer> framebuffers,
                        vk::Extent2D renderArea);
            ~Framebuffer();

            [[nodiscard]] const vk::Framebuffer &getFramebuffer(const std::uint32_t &imageIdx) const {
                return this->_framebuffers.at(imageIdx);
            }

            [[nodiscard]] const vk::Extent2D &getRenderArea() const {
                return this->_renderArea;
            }
        };

        struct GraphState {
            RenderGraph graph;
            std::vector<std::unique_ptr<Pass>> passes;
        };

        struct FramebufferState {
            std::vector<std::unique_ptr<Target>> targets;
            std::vector<std::unique_ptr<Framebuffer>> framebuffers;
        };

        ResourceSet *_resources;
        EventQueue *_eventQueue;
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
        RenderContext *_renderContext;
        RenderListProvider *_renderListProvider;

        std::array<vk::CommandBuffer, INFLIGHT_FRAME_COUNT> _commandBuffers;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _graphExecutedSemaphores;

        std::mutex _executorMutex;
        std::optional<EventHandlerIndex> _eventHandlerIdx;
        std::optional<GraphState> _graphState;
        std::optional<FramebufferState> _framebufferState;

        std::unique_ptr<Target> createTarget(const RenderTarget &target);
        std::unique_ptr<Pass> createPass(const RenderSubgraph &subgraph);
        std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferState &framebufferState,
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
