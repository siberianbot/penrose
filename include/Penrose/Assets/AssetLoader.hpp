#ifndef PENROSE_ASSETS_ASSET_LOADER_HPP
#define PENROSE_ASSETS_ASSET_LOADER_HPP

#include <filesystem>
#include <functional>
#include <map>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Assets/AssetReader.hpp>
#include <Penrose/Assets/AssetType.hpp>
#include <Penrose/Assets/ImageAssetFactory.hpp>
#include <Penrose/Assets/MeshAssetFactory.hpp>
#include <Penrose/Assets/ShaderAssetFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class AssetLoader : public Resource {
    public:
        explicit AssetLoader(ResourceSet *resources);
        ~AssetLoader() override = default;

        [[nodiscard]] Asset *load(const std::filesystem::path &path) const;

    private:
        using Provider = std::function<Asset *(AssetReader &)>;

        Lazy<ImageAssetFactory> _imageAssetFactory;
        Lazy<MeshAssetFactory> _meshAssetFactory;
        Lazy<ShaderAssetFactory> _shaderAssetFactory;

        std::map<AssetType, Provider> _providers;
    };
}

#endif // PENROSE_ASSETS_ASSET_LOADER_HPP
