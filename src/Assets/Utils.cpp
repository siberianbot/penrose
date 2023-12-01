#include "Utils.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    inline static constexpr std::string_view INDEX_FILENAME = "index";

    IndexFile readIndexFile(std::filesystem::path &&rootDir) {
        const auto indexPath = rootDir / INDEX_FILENAME;

        if (!std::filesystem::exists(indexPath)) {
            throw EngineError("Index file not found");
        }

        auto indexStream = std::ifstream(indexPath);

        if (!indexStream.good()) {
            throw EngineError("Failed to read index file");
        }

        nlohmann::json indexJson;

        indexStream >> indexJson;

        if (!indexJson.is_object()) {
            throw EngineError("Root element is not an object");
        }

        IndexFile index;

        for (const auto &[asset, entryJson]: indexJson.items()) {
            if (!entryJson.is_object()) {
                throw EngineError("Entry for asset {} is not an object", asset);
            }

            auto entry = IndexFileEntry {
                .path = (rootDir / entryJson.at("path").get<std::filesystem::path>()).lexically_normal(),
                .preload = entryJson.at("preload").get<bool>(),
            };

            index.insert_or_assign(asset, std::move(entry));
        }

        return index;
    }
}
