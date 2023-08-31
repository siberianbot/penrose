#include <Penrose/Assets/AssetReader.hpp>

#include <utility>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    AssetReader::AssetReader(std::filesystem::path path)
            : _path(std::move(path)) {
        //
    }

    void AssetReader::open() {
        if (this->_stream.is_open()) {
            return;
        }

        this->_stream = std::ifstream(this->_path, std::ios::binary);

        if (!this->_stream.is_open()) {
            throw EngineError(fmt::format("Failed to open {}", this->_path.string()));
        }
    }

    void AssetReader::read(std::size_t size, void *ptr) {
        this->_stream.read(reinterpret_cast<char *>(ptr), static_cast<std::streamsize>(size));

        if (!this->_stream.good()) {
            throw EngineError(fmt::format("Failed to read {}", this->_path.string()));
        }
    }
}
