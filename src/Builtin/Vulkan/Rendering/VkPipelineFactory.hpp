#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_FACTORY_HPP

#include <map>
#include <string>
#include <tuple>

#include <vulkan/vulkan.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    class VkPipelineFactory : public Resource<VkPipelineFactory>,
                              public Initializable,
                              public PipelineFactory {
    public:
        explicit VkPipelineFactory(const ResourceSet *resources);
        ~VkPipelineFactory() override = default;

        void init() override;
        void destroy() override;

        void addPipeline(const std::string &pipelineName, const PipelineInfo &pipelineInfo) override;

        [[nodiscard]] Pipeline *getPipeline(const std::string &pipelineName,
                                            const RenderSubgraph *subgraph,
                                            std::uint32_t passIdx) override;

    private:
        using PipelineKey = std::tuple<std::string, const RenderSubgraph *, std::uint32_t>;

        ResourceProxy<AssetManager> _assetManager;
        ResourceProxy<VkDescriptorPoolManager> _descriptorPoolManager;
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;

        vk::PipelineCache _pipelineCache;
        std::map<std::string, PipelineInfo> _pipelineInfos;
        std::map<PipelineKey, Pipeline *> _pipelines;

        [[nodiscard]] Pipeline *createPipeline(const std::string &pipelineName,
                                               const VkRenderSubgraph *subgraph,
                                               std::uint32_t passIdx);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_FACTORY_HPP
