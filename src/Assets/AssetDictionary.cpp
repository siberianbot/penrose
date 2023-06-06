#include "AssetDictionary.hpp"

#include <fmt/core.h>

#include "src/Common/EngineError.hpp"
#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    void AssetDictionary::addDir(const std::filesystem::path &currentPath, // NOLINT(misc-no-recursion)
                                 const std::filesystem::path &root) {
        for (const auto &entry: std::filesystem::directory_iterator(currentPath)) {
            if (std::filesystem::is_directory(entry)) {
                this->addDir(entry, root);
            } else {
                auto asset = std::filesystem::relative(entry, root).string();
                this->addFile(asset, entry);
            }
        }
    }

    void AssetDictionary::addFile(const Asset &asset, const std::filesystem::path &path) {
        if (std::filesystem::is_directory(path)) {
            throw EngineError(fmt::format("Path {} is a directory", path.string()));
        }

        this->_assets.insert_or_assign(asset, path);
    }

    void AssetDictionary::addDir(const std::filesystem::path &path) {
        if (!std::filesystem::is_directory(path)) {
            throw EngineError(fmt::format("Path {} is not a directory", path.string()));
        }

        this->addDir(path, path);
    }

    std::optional<std::filesystem::path> AssetDictionary::tryGetPath(const Asset &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::filesystem::path AssetDictionary::getPath(const Asset &asset) const {
        return orElseThrow(this->tryGetPath(asset), EngineError(fmt::format("Asset {} not found", asset)));
    }
}
