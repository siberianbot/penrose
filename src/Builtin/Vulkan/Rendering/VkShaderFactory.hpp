#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP

#include <Penrose/Rendering/ShaderFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkShaderFactory : public Resource, public ShaderFactory {
    public:
        explicit VkShaderFactory(ResourceSet *resources);
        ~VkShaderFactory() override = default;

        Shader *makeShader(const std::uint32_t *data, std::uint32_t size) override;

    private:
        Lazy<DeviceContext> _deviceContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_FACTORY_HPP
