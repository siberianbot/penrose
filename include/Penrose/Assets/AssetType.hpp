#ifndef PENROSE_ASSETS_ASSET_TYPE_HPP
#define PENROSE_ASSETS_ASSET_TYPE_HPP

#include <cstdint>

namespace Penrose {

    /**
     * \brief Type of asset
     */
    enum class AssetType : std::uint8_t {
        Shader = 0x01,
        Mesh = 0x02,
        Image = 0x03,
        UILayout = 0x04
    };
}

#endif // PENROSE_ASSETS_ASSET_TYPE_HPP
