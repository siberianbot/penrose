#ifndef PENROSE_ASSETS_ASSET_DICTIONARY_HPP
#define PENROSE_ASSETS_ASSET_DICTIONARY_HPP

#include <filesystem>
#include <optional>
#include <unordered_map>

#include <Penrose/Resources/Resource.hpp>

#include "src/Assets/AssetId.hpp"
#include "src/Assets/AssetInstance.hpp"

namespace Penrose {

    class AssetDictionary : public Resource {
    private:
        std::unordered_map<AssetId, std::filesystem::path> _assets;

        void addDir(const std::filesystem::path &currentPath, const std::filesystem::path &root);

    public:
        ~AssetDictionary() override = default;

        void addFile(const AssetId &asset, const std::filesystem::path &path);
        void addDir(const std::filesystem::path &path);

        [[nodiscard]] std::optional<std::filesystem::path> tryGetPath(const AssetId &asset) const;
        [[nodiscard]] std::filesystem::path getPath(const AssetId &asset) const;

        [[nodiscard]] AssetInstance openAsset(const AssetId &asset) const;
    };
}

#endif // PENROSE_ASSETS_ASSET_DICTIONARY_HPP
