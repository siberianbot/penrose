#ifndef PENROSE_ASSETS_SHADER_ASSET_HPP
#define PENROSE_ASSETS_SHADER_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Shader.hpp>

namespace Penrose {

    class ShaderAsset : public Asset {
    public:
        ShaderAsset(std::unique_ptr<Shader> shader);
        ~ShaderAsset() override = default;

        [[nodiscard]] AssetType getType() const override { return AssetType::Shader; }

        [[nodiscard]] Shader *getShader() const { return this->_shader.get(); }

    private:
        std::unique_ptr<Shader> _shader;
    };
}

#endif // PENROSE_ASSETS_SHADER_ASSET_HPP
