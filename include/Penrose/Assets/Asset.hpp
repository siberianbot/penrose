#ifndef PENROSE_ASSETS_ASSET_HPP
#define PENROSE_ASSETS_ASSET_HPP

namespace Penrose {

    enum class AssetType {
        Common,
        Shader,
        Mesh,
        Image
    };

    class Asset {
    public:
        virtual ~Asset() = default;

        [[nodiscard]] virtual AssetType getType() const { return AssetType::Common; }
    };

    class ShaderAsset : public Asset {
    public:
        ~ShaderAsset() override = default;

        [[nodiscard]] AssetType getType() const override { return AssetType::Shader; }
    };
}

#endif // PENROSE_ASSETS_ASSET_HPP
