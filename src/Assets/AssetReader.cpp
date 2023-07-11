#include "AssetReader.hpp"

#include <fstream>
#include <utility>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    AssetReader::AssetReader(std::filesystem::path path)
            : _path(std::move(path)) {
        //
    }

    void AssetReader::read() {
        auto stream = std::ifstream(this->_path, std::ios::ate | std::ios::binary);

        if (!stream.is_open()) {
            throw EngineError(fmt::format("Failed to open asset {}", this->_path.string()));
        }

        auto length = stream.tellg();
        this->_data = std::vector<std::byte>(length);

        stream.seekg(0, std::ios::beg);
        stream.read(reinterpret_cast<char *>(this->_data->data()), length);

        if (!stream.good()) {
            throw EngineError(fmt::format("Failed to read asset {}", this->_path.string()));
        }
    }

    AssetReader AssetReader::read(std::filesystem::path &&path) {
        auto reader = AssetReader(path);
        reader.read();

        return reader;
    }
}
