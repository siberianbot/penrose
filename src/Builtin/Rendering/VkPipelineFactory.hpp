#ifndef PENROSE_BUILTIN_RENDERING_VK_PIPELINE_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_PIPELINE_FACTORY_HPP

#include <map>
#include <string>
#include <tuple>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class DeviceContext;

    class VkPipelineFactory : public Resource, public Initializable, public PipelineFactory {
    public:
        explicit VkPipelineFactory(ResourceSet *resources);
        ~VkPipelineFactory() override = default;

        void init() override;
        void destroy() override;

        void addPipeline(const std::string &pipelineName, const PipelineInfo &pipelineInfo) override;

        [[nodiscard]] Pipeline *getPipeline(const std::string &pipelineName,
                                            const RenderSubgraph *subgraph,
                                            std::uint32_t passIdx) override;

    private:
        using PipelineKey = std::tuple<std::string, const RenderSubgraph *, std::uint32_t>;

        AssetManager *_assetManager;
        DeviceContext *_deviceContext;

        vk::PipelineCache _pipelineCache;
        std::map<std::string, PipelineInfo> _pipelineInfos;
        std::map<PipelineKey, Pipeline *> _pipelines;

        [[nodiscard]] Pipeline *createPipeline(const std::string &pipelineName,
                                               const VkRenderSubgraph *subgraph,
                                               std::uint32_t passIdx) const;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_PIPELINE_FACTORY_HPP
