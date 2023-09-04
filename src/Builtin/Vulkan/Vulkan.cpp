#include <Penrose/Builtin/Vulkan.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Assets/VkImageAssetFactory.hpp"
#include "src/Builtin/Vulkan/Assets/VkMeshAssetFactory.hpp"
#include "src/Builtin/Vulkan/Assets/VkShaderAssetFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSamplerFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkShaderFactory.hpp"

namespace Penrose {

    ResourceSet &addVulkan(ResourceSet &resources) {

        resources.add<VulkanBackend>(resources.getBeginIterator());

        resources.add<DeviceContext>();
        resources.add<PresentContext>();

        resources.add<VkImageAssetFactory, ImageAssetFactory>();
        resources.add<VkMeshAssetFactory, MeshAssetFactory>();
        resources.add<VkShaderAssetFactory, ShaderAssetFactory>();

        resources.add<VkBufferFactory, BufferFactory>();
        resources.add<VkImageFactory, ImageFactory>();
        resources.add<VkPipelineFactory, PipelineFactory>();
        resources.add<VkRenderSubgraphFactory, RenderSubgraphFactory>();
        resources.add<VkRenderTargetFactory, RenderTargetFactory>();
        resources.add<VkSamplerFactory, SamplerFactory>();
        resources.add<VkShaderFactory, ShaderFactory>();

        return resources;
    }
}