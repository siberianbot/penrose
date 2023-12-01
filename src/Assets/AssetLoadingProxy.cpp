#include "AssetLoadingProxy.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Assets/AssetReader.hpp"
#include "src/Assets/Structs.hpp"

namespace Penrose {

    AssetLoadingProxy::AssetLoadingProxy(ResourceSet *resources)
        : _loaders(resources->get<TypedAssetLoader>()) {
        for (const auto &loader: this->_loaders) {
            this->_loadersMap.emplace(loader->getAssetType(), loader);
        }
    }

    Asset *AssetLoadingProxy::fromPath(std::filesystem::path &&path) {
        auto reader = AssetReader(std::forward<decltype(path)>(path));

        reader.open();

        if (const auto [value] = reader.read<MagicHeader>(); !std::ranges::equal(value, ASSET_MAGIC)) {
            throw EngineError("File is not an asset");
        }

        if (const auto [value] = reader.read<VersionHeader>(); value != ASSET_VERSION) {
            throw EngineError("Asset of version v{} is not supported", value);
        }

        const auto [type] = reader.read<V1Header>();
        const auto loaderIt = this->_loadersMap.find(type);

        if (loaderIt == this->_loadersMap.end()) {
            throw EngineError("Asset type {} is not supported", static_cast<std::uint8_t>(type));
        }

        return loaderIt->second->fromReader(reader);
    }
}
