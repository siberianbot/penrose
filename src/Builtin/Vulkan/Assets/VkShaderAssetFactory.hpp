#ifndef PENROSE_BUILTIN_VULKAN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP

#include <Penrose/Assets/ShaderAssetFactory.hpp>
#include <Penrose/Rendering/ShaderFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class VkShaderAssetFactory : public Resource, public ShaderAssetFactory {
    public:
        explicit VkShaderAssetFactory(ResourceSet *resources);
        ~VkShaderAssetFactory() override = default;

        [[nodiscard]] ShaderAsset *makeShader(const std::uint32_t *data, std::uint32_t size) override;

    private:
        Lazy<ShaderFactory> _shaderFactory;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP
