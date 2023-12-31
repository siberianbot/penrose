#ifndef PENROSE_ASSETS_STRUCTS_HPP
#define PENROSE_ASSETS_STRUCTS_HPP

#include <cstdint>

#include <Penrose/Assets/AssetType.hpp>
#include <Penrose/Rendering/ImageFormat.hpp>

namespace Penrose {

    constexpr char ASSET_MAGIC[4] = {'P', 'n', 'r', 's'};
    constexpr std::uint8_t ASSET_VERSION = 0x01;

#pragma pack(push, 1)

    struct MagicHeader {
        char value[4];
    };

    struct VersionHeader {
        std::uint8_t value;
    };

    struct V1Header {
        AssetType type;
    };

    struct MeshInfo {
        std::uint32_t verticesCount;
        std::uint32_t indicesCount;
    };

    struct ImageInfo {
        std::uint32_t width;
        std::uint32_t height;
        ImageFormat format;
        std::uint32_t size;
    };

    struct ShaderInfo {
        std::uint32_t size;
    };

    struct UILayoutInfo {
        std::uint32_t size;
    };

#pragma pack(pop)
}

#endif // PENROSE_ASSETS_STRUCTS_HPP
