#ifndef PENROSE_ASSETS_ASSET_TYPE_HPP
#define PENROSE_ASSETS_ASSET_TYPE_HPP

#include <cstdint>

namespace Penrose {

    enum class AssetType : std::uint8_t {
        Unknown = 0x00,
        Shader = 0x01,
        Mesh = 0x02,
        Image = 0x03
    };
}

#endif // PENROSE_ASSETS_ASSET_TYPE_HPP
