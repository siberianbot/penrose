#ifndef PENROSE_ASSETS_ASSET_MANAGER_HPP
#define PENROSE_ASSETS_ASSET_MANAGER_HPP

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Assets/AssetId.hpp>
#include <Penrose/Common/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetDictionary;
    class DeviceContext;

    class AssetManager : public Resource {
    private:
        AssetDictionary *_assetDictionary;
        DeviceContext *_deviceContext;

    public:
        explicit AssetManager(ResourceSet *resources);
        ~AssetManager() override = default;

        [[nodiscard]] ShaderAsset loadShader(const AssetId &asset) const;
        [[nodiscard]] MeshAsset loadMesh(const AssetId &asset) const;
    };
}

#endif // PENROSE_ASSETS_ASSET_MANAGER_HPP
