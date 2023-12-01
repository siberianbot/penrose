#include "AssetReader.hpp"

#include <utility>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    AssetReader::AssetReader(std::filesystem::path &&path)
        : _path(std::forward<decltype(path)>(path)) {
        //
    }

    void AssetReader::open() {
        if (this->_stream.is_open()) {
            return;
        }

        this->_stream = std::ifstream(this->_path, std::ios::binary);

        if (!this->_stream.is_open()) {
            throw EngineError("Failed to open {}", this->_path.string());
        }
    }

    void AssetReader::read(const std::size_t size, void *ptr) {
        this->_stream.read(static_cast<char *>(ptr), static_cast<std::streamsize>(size));

        if (!this->_stream.good()) {
            throw EngineError("Failed to read {}", this->_path.string());
        }
    }
}
