#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_FACTORY_HPP

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/Objects/PipelineFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipelineInstance.hpp"

namespace Penrose {

    class VkPipelineFactory final: public Resource<VkPipelineFactory>,
                                   public PipelineFactory {
    public:
        explicit VkPipelineFactory(const ResourceSet *resources);
        ~VkPipelineFactory() override = default;

        void init();
        void destroy();

        [[nodiscard]] Pipeline *makePipeline(PipelineInfo &&pipelineInfo) override;

        [[nodiscard]] VkPipelineInstance *makePipelineInstance(
            const VkPipeline *pipeline, vk::RenderPass pass, std::uint32_t subpass
        );

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<AssetManager> _assetManager;
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;

        vk::UniquePipelineCache _cache;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_FACTORY_HPP
