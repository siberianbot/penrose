#ifndef PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP

#include <Penrose/Assets/ShaderAssetFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class ShaderFactory;

    class VkShaderAssetFactory : public ShaderAssetFactory {
    public:
        explicit VkShaderAssetFactory(ResourceSet *resources);
        ~VkShaderAssetFactory() override = default;

        [[nodiscard]] ShaderAsset *makeShader(const std::uint32_t *data, std::uint32_t size) override;

    private:
        ShaderFactory *_shaderFactory;
    };
}

#endif // PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_FACTORY_HPP
