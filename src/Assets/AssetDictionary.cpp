#include <Penrose/Assets/AssetDictionary.hpp>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    void AssetDictionary::addFile(std::string &&asset, std::filesystem::path &&path) {
        if (std::filesystem::is_directory(path)) {
            throw EngineError(fmt::format("Path {} is a directory", path.string()));
        }

        this->_assets.insert_or_assign(asset, path);
    }

    void AssetDictionary::addDir(std::filesystem::path &&path) {
        if (!std::filesystem::is_directory(path)) {
            throw EngineError(fmt::format("Path {} is not a directory", path.string()));
        }

        auto root = path;
        this->addDir(std::move(path), root);
    }

    std::optional<std::filesystem::path> AssetDictionary::tryGetPath(const std::string &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::filesystem::path AssetDictionary::getPath(const std::string &asset) const {
        return orElseThrow(this->tryGetPath(asset), EngineError(fmt::format("Asset {} not found", asset)));
    }

    void AssetDictionary::addDir(std::filesystem::path &&currentPath, // NOLINT(misc-no-recursion)
                                 const std::filesystem::path &root) {
        for (const auto &entry: std::filesystem::directory_iterator(currentPath)) {
            auto path = entry.path();

            if (std::filesystem::is_directory(entry)) {
                this->addDir(std::move(path), root);
            } else {
                std::string asset = std::filesystem::relative(entry, root).string();
                this->addFile(std::move(asset), std::move(path));
            }
        }
    }
}
