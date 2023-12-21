#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_INSTANCE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_INSTANCE_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkPipeline.hpp"

namespace Penrose {

    class VkPipelineInstance {
    public:
        VkPipelineInstance(
            const VkPipeline *pipeline, const vk::RenderPass pass, const std::uint32_t subpass,
            vk::UniquePipeline &&handle
        )
            : _pipeline(pipeline),
              _pass(pass),
              _subpass(subpass),
              _handle(std::forward<decltype(handle)>(handle)) {
            //
        }

        ~VkPipelineInstance() = default;

        [[nodiscard]] const VkPipeline *getPipeline() const { return this->_pipeline; }

        [[nodiscard]] const vk::RenderPass &getPass() const { return this->_pass; }

        [[nodiscard]] std::uint32_t getSubpass() const { return this->_subpass; }

        [[nodiscard]] const vk::Pipeline &getHandle() const { return this->_handle.get(); }

    private:
        const VkPipeline *_pipeline;
        vk::RenderPass _pass;
        std::uint32_t _subpass;

        vk::UniquePipeline _handle;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_INSTANCE_HPP
