#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_EXECUTOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_EXECUTOR_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderGraphExecutor : public Resource, public Initializable {
    public:
        struct Submit {
            vk::CommandBuffer commandBuffer;
            std::vector<vk::Semaphore> waitSemaphores;
            std::vector<vk::Semaphore> signalSemaphores;
        };

        explicit VkRenderGraphExecutor(ResourceSet *resources);
        ~VkRenderGraphExecutor() override = default;

        void init() override;

        void destroy() override;

        [[nodiscard]] std::vector<Submit> execute(VkRenderGraphContext *graphContext,
                                                  const vk::CommandBuffer &commandBuffer,
                                                  const vk::Semaphore &waitSemaphore,
                                                  const vk::Semaphore &signalSemaphore,
                                                  std::uint32_t frameIdx, std::uint32_t imageIdx);

    private:
        LazyCollection<RenderOperator> _renderOperators;
        Lazy<VkSwapchainManager> _swapchainManager;

        std::map<std::string, RenderOperator *> _renderOperatorsMap;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_EXECUTOR_HPP