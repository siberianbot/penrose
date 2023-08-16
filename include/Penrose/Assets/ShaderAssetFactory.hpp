#ifndef PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP
#define PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP

#include <cstdint>

#include <Penrose/Assets/ShaderAsset.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ShaderAssetFactory : public Resource {
    public:
        ~ShaderAssetFactory() override = default;

        [[nodiscard]] virtual ShaderAsset *makeShader(const std::uint32_t *data, std::uint32_t size) = 0;
    };
}

#endif // PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP
