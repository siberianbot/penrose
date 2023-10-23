#include <Penrose/Assets/AssetDictionary.hpp>

#include <fstream>
#include <string>
#include <string_view>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    constexpr static const std::string_view ASSET_DICTIONARY_TAG = "AssetDictionary";
    constexpr static const std::string_view INDEX_FILENAME = "index";

    AssetDictionary::AssetDictionary(ResourceSet *resources)
            : _log(resources->get<Log>()) {
        //
    }

    void AssetDictionary::addFile(std::string &&asset, std::filesystem::path &&path) {
        if (!std::filesystem::is_regular_file(path)) {
            throw EngineError("Path {} is a directory", path.string());
        }

        this->_assets.insert_or_assign(asset, path);
    }

    void AssetDictionary::addDir(std::filesystem::path &&path) {
        if (!std::filesystem::is_directory(path)) {
            throw EngineError("Path {} is not a directory", path.string());
        }

        std::map<std::string, std::filesystem::path> index;

        try {
            index = this->readIndex(path);
        } catch (const std::exception &error) {
            std::throw_with_nested(EngineError("Failed to add directory {} to asset dictionary", path.string()));
        }

        this->_assets.merge(index);
    }

    std::optional<std::filesystem::path> AssetDictionary::tryGetPath(const std::string &asset) const {
        auto it = this->_assets.find(asset);

        if (it == this->_assets.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::filesystem::path AssetDictionary::getPath(const std::string &asset) const {
        return orElseThrow(this->tryGetPath(asset), EngineError("Asset {} not found", asset));
    }

    AssetDictionary::IndexMap AssetDictionary::readIndex(const std::filesystem::path &root) {
        auto indexPath = root / INDEX_FILENAME;

        if (!std::filesystem::exists(indexPath) || !std::filesystem::is_regular_file(indexPath)) {
            throw EngineError("Index file not found");
        }

        auto indexStream = std::ifstream(indexPath);

        if (!indexStream.good()) {
            throw EngineError("Failed to read index file");
        }

        IndexMap index;

        for (std::string line; std::getline(indexStream, line);) {

            auto delim = line.find_first_of(':');

            std::string asset = line.substr(0, delim);
            std::filesystem::path path = line.substr(delim + 1, line.size() - delim - 1);

            if (index.contains(asset)) {
                this->_log->writeWarning(ASSET_DICTIONARY_TAG,
                                         "Index file {} contains duplicated asset {}",
                                         indexPath.string(), asset);
            }

            index.insert_or_assign(asset, root / path);
        }

        return index;
    }
}
