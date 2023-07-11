#ifndef PENROSE_ASSETS_ASSET_READER_HPP
#define PENROSE_ASSETS_ASSET_READER_HPP

#include <filesystem>
#include <optional>
#include <vector>

namespace Penrose {

    class AssetReader {
    public:
        explicit AssetReader(std::filesystem::path path);

        void read();

        [[nodiscard]] bool ready() const { return this->_data.has_value(); }

        [[nodiscard]] const std::byte *data() const { return this->_data->data(); }

        [[nodiscard]] std::size_t size() const { return this->_data->size(); }

        static AssetReader read(std::filesystem::path &&path);

    private:
        std::filesystem::path _path;
        std::optional<std::vector<std::byte>> _data;
    };
}

#endif // PENROSE_ASSETS_ASSET_READER_HPP
