#ifndef PENROSE_BUILTIN_RENDERING_VK_SHADER_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_SHADER_FACTORY_HPP

#include <Penrose/Rendering/ShaderFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;

    class VkShaderFactory : public Resource, public ShaderFactory {
    public:
        explicit VkShaderFactory(ResourceSet *resources);
        ~VkShaderFactory() override = default;

        Shader *makeShader(const std::uint32_t *data, std::uint32_t size) override;

    private:
        DeviceContext *_deviceContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_SHADER_FACTORY_HPP
