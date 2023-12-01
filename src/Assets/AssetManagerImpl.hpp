#ifndef PENROSE_ASSETS_ASSET_MANAGER_IMPL_HPP
#define PENROSE_ASSETS_ASSET_MANAGER_IMPL_HPP

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/AssetIndex.hpp"
#include "src/Assets/AssetLoadingJobQueue.hpp"

namespace Penrose {

    class AssetManagerImpl final: public Resource<AssetManagerImpl, ResourceGroup::Assets>,
                                  public Initializable,
                                  public AssetManager {
    public:
        explicit AssetManagerImpl(ResourceSet *resources);
        ~AssetManagerImpl() override = default;

        void init() override;
        void destroy() override;

        void addDir(std::filesystem::path &&rootDir) override;
        void tryAddDir(std::filesystem::path &&rootDir) override;

        void load(std::string_view &&asset) override;
        void unload(std::string_view &&asset) override;

        [[nodiscard]] std::shared_ptr<Asset> getAsset(std::string_view &&asset) override;
        [[nodiscard]] std::optional<std::shared_ptr<Asset>> tryGetAsset(std::string_view &&asset) override;

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<AssetIndex> _assetIndex;
        ResourceProxy<AssetLoadingJobQueue> _assetLoadingJobQueue;
    };
}

#endif // PENROSE_ASSETS_ASSET_MANAGER_IMPL_HPP
