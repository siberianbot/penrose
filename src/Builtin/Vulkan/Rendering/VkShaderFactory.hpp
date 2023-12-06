#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP

#include <Penrose/Rendering/ShaderFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    class VkShaderFactory : public Resource<VkShaderFactory>,
                            public ShaderFactory {
    public:
        explicit VkShaderFactory(const ResourceSet *resources);
        ~VkShaderFactory() override = default;

        Shader *makeShader(const std::uint32_t *data, std::uint32_t size) override;

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
