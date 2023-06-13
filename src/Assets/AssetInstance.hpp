#ifndef PENROSE_ASSETS_ASSET_INSTANCE_HPP
#define PENROSE_ASSETS_ASSET_INSTANCE_HPP

#include <filesystem>
#include <optional>
#include <vector>

#include "src/Assets/AssetId.hpp"

namespace Penrose {

    class AssetInstance {
    private:
        using Data = std::vector<std::byte>;

        AssetId _asset;
        std::filesystem::path _path;
        std::optional<Data> _data;

    public:
        AssetInstance(AssetId asset, std::filesystem::path path);

        void read();

        [[nodiscard]] bool ready() const { return this->_data.has_value(); }

        [[nodiscard]] const std::byte *data() const { return this->_data.value().data(); }

        [[nodiscard]] std::size_t size() const { return this->_data.value().size(); }
    };
}

#endif // PENROSE_ASSETS_ASSET_INSTANCE_HPP
