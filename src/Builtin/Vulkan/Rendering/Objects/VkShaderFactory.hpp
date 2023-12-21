#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP

#include <Penrose/Rendering/Objects/ShaderFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"

namespace Penrose {

    class VkShaderFactory final: public Resource<VkShaderFactory>,
                                 public ShaderFactory {
    public:
        explicit VkShaderFactory(const ResourceSet *resources);
        ~VkShaderFactory() override = default;

        [[nodiscard]] Shader *makeShader(std::vector<std::byte> &&rawData) override;

    private:
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
