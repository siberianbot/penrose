#include "AssetInstance.hpp"

#include <fstream>
#include <utility>

#include <fmt/core.h>

#include "src/Common/EngineError.hpp"

namespace Penrose {

    AssetInstance::AssetInstance(AssetId asset, std::filesystem::path path)
            : _asset(std::move(asset)),
              _path(std::move(path)) {
        //
    }

    void AssetInstance::read() {
        auto stream = std::ifstream(this->_path, std::ios::ate | std::ios::binary);

        if (!stream.is_open()) {
            throw EngineError(fmt::format("Failed to open asset {} @ {}", this->_asset, this->_path.string()));
        }

        auto length = stream.tellg();
        auto data = Data(length);

        stream.seekg(0, std::ios::beg);
        stream.read(reinterpret_cast<char *>(data.data()), length);

        if (!stream.good()) {
            throw EngineError(fmt::format("Failed to read asset {} @ {}", this->_asset, this->_path.string()));
        }

        this->_data = data;
    }
}
