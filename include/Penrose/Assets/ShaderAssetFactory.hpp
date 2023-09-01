#ifndef PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP
#define PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP

#include <cstdint>

#include <Penrose/Assets/ShaderAsset.hpp>

namespace Penrose {

    class ShaderAssetFactory {
    public:
        virtual ~ShaderAssetFactory() = default;

        [[nodiscard]] virtual ShaderAsset *makeShader(const std::uint32_t *data, std::uint32_t size) = 0;
    };
}

#endif // PENROSE_ASSETS_SHADER_ASSET_FACTORY_HPP
