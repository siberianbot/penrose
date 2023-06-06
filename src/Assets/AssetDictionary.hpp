#ifndef PENROSE_ASSETS_ASSET_DICTIONARY_HPP
#define PENROSE_ASSETS_ASSET_DICTIONARY_HPP

#include <filesystem>
#include <optional>
#include <unordered_map>

#include "src/Assets/Asset.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class AssetDictionary : public Resource {
    private:
        std::unordered_map<Asset, std::filesystem::path> _assets;

        void addDir(const std::filesystem::path &currentPath, const std::filesystem::path &root);

    public:
        ~AssetDictionary() override = default;

        void addFile(const Asset &asset, const std::filesystem::path &path);
        void addDir(const std::filesystem::path &path);

        [[nodiscard]] std::optional<std::filesystem::path> tryGetPath(const Asset &asset) const;
        [[nodiscard]] std::filesystem::path getPath(const Asset &asset) const;
    };
}

#endif // PENROSE_ASSETS_ASSET_DICTIONARY_HPP
