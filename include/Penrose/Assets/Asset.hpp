#ifndef PENROSE_ASSETS_ASSET_HPP
#define PENROSE_ASSETS_ASSET_HPP

#include <cstdint>

#include <Penrose/Rendering/Utils.hpp>

namespace Penrose {

    struct Asset {
        //
    };

    struct ShaderAsset : public Asset {
        ShaderModule shaderModule;
    };

    struct MeshAsset : public Asset {
        Buffer vertexBuffer;
        DeviceMemory vertexBufferMemory;
        Buffer indexBuffer;
        DeviceMemory indexBufferMemory;
        std::uint32_t indexCount;
    };
}

#endif // PENROSE_ASSETS_ASSET_HPP
