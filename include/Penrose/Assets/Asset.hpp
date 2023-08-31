#ifndef PENROSE_ASSETS_ASSET_HPP
#define PENROSE_ASSETS_ASSET_HPP

#include <Penrose/Assets/AssetType.hpp>

namespace Penrose {

    class Asset {
    public:
        virtual ~Asset() = default;

        [[nodiscard]] virtual AssetType getType() const = 0;
    };
}

#endif // PENROSE_ASSETS_ASSET_HPP
