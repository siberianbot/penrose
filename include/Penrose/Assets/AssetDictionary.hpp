#ifndef PENROSE_ASSETS_ASSET_DICTIONARY_HPP
#define PENROSE_ASSETS_ASSET_DICTIONARY_HPP

#include <filesystem>
#include <map>
#include <optional>
#include <string>

#include <Penrose/Api.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class PENROSE_API AssetDictionary : public Resource {
    public:
        explicit AssetDictionary(ResourceSet *resources);
        ~AssetDictionary() override = default;

        void addFile(std::string &&asset, std::filesystem::path &&path);
        void addDir(std::filesystem::path &&path);

        [[nodiscard]] std::optional<std::filesystem::path> tryGetPath(const std::string &asset) const;
        [[nodiscard]] std::filesystem::path getPath(const std::string &asset) const;

    private:
        using IndexMap = std::map<std::string, std::filesystem::path>;

        Lazy<Log> _log;

        IndexMap _assets;

        [[nodiscard]] IndexMap readIndex(const std::filesystem::path &root);
    };
}

#endif // PENROSE_ASSETS_ASSET_DICTIONARY_HPP
