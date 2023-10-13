#ifndef PENROSE_ASSETS_ASSET_DICTIONARY_HPP
#define PENROSE_ASSETS_ASSET_DICTIONARY_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

#include <Penrose/Api.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class PENROSE_API AssetDictionary : public Resource {
    public:
        ~AssetDictionary() override = default;

        void addFile(std::string &&asset, std::filesystem::path &&path);
        void addDir(std::filesystem::path &&path);

        [[nodiscard]] std::optional<std::filesystem::path> tryGetPath(const std::string &asset) const;
        [[nodiscard]] std::filesystem::path getPath(const std::string &asset) const;

    private:
        std::unordered_map<std::string, std::filesystem::path> _assets;

        void addDir(std::filesystem::path &&currentPath, const std::filesystem::path &root);
    };
}

#endif // PENROSE_ASSETS_ASSET_DICTIONARY_HPP
