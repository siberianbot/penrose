#ifndef PENROSE_ASSETS_ASSET_READER_HPP
#define PENROSE_ASSETS_ASSET_READER_HPP

#include <cstdint>
#include <filesystem>
#include <fstream>

namespace Penrose {

    class AssetReader {
    public:
        explicit AssetReader(std::filesystem::path path);

        void open();
        void read(std::size_t size, void *ptr);

        template<typename T>
        [[nodiscard]] T read();

    private:
        std::filesystem::path _path;
        std::ifstream _stream;
    };
}

#include "AssetReader.inl"

#endif // PENROSE_ASSETS_ASSET_READER_HPP
