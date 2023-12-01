#ifndef PENROSE_ASSETS_ASSET_LOADING_PROXY_HPP
#define PENROSE_ASSETS_ASSET_LOADING_PROXY_HPP

#include <map>

#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class AssetLoadingProxy final: public Resource<AssetLoadingProxy, ResourceGroup::Assets> {
    public:
        explicit AssetLoadingProxy(ResourceSet *resources);
        ~AssetLoadingProxy() override = default;

        [[nodiscard]] Asset *fromPath(std::filesystem::path &&path);

    private:
        ResourceProxy<TypedAssetLoader> _loaders;

        std::map<AssetType, TypedAssetLoader *> _loadersMap;
    };
}

#endif // PENROSE_ASSETS_ASSET_LOADING_PROXY_HPP
