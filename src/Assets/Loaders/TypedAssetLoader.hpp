#ifndef PENROSE_ASSETS_LOADERS_TYPED_ASSET_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_TYPED_ASSET_LOADER_HPP

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Assets/AssetType.hpp>

#include "src/Assets/AssetReader.hpp"

namespace Penrose {

    class TypedAssetLoader {
    public:
        virtual ~TypedAssetLoader() = default;

        [[nodiscard]] virtual AssetType getAssetType() const = 0;

        [[nodiscard]] virtual Asset *fromReader(AssetReader &reader) = 0;
    };
}

#endif // PENROSE_ASSETS_LOADERS_TYPED_ASSET_LOADER_HPP
