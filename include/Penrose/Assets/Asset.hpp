#ifndef PENROSE_ASSETS_ASSET_HPP
#define PENROSE_ASSETS_ASSET_HPP

#include <Penrose/Assets/AssetType.hpp>

namespace Penrose {

    /**
     * \brief Interface of asset - engine resource (mesh, texture, etc)
     */
    class PENROSE_API Asset {
    public:
        virtual ~Asset() = default;

        /**
         * \brief Returns type of asset
         * \return type of asset
         */
        [[nodiscard]] virtual AssetType getType() const = 0;
    };
}

#endif // PENROSE_ASSETS_ASSET_HPP
