#ifndef PENROSE_ASSETS_ASSET_HPP
#define PENROSE_ASSETS_ASSET_HPP

#include "src/Rendering/Utils.hpp"

namespace Penrose {

    struct Asset {
        //
    };

    struct ShaderAsset : public Asset {
        ShaderModule shaderModule;
    };
}

#endif // PENROSE_ASSETS_ASSET_HPP
