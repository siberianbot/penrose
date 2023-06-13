#ifndef PENROSE_ASSETS_ASSET_MANAGER_HPP
#define PENROSE_ASSETS_ASSET_MANAGER_HPP

#include "src/Assets/Asset.hpp"
#include "src/Assets/AssetId.hpp"
#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

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
    };
}

#endif // PENROSE_ASSETS_ASSET_MANAGER_HPP
