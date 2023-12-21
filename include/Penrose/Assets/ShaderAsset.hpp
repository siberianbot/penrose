#ifndef PENROSE_ASSETS_SHADER_ASSET_HPP
#define PENROSE_ASSETS_SHADER_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Objects/Shader.hpp>

namespace Penrose {

    /**
     * \brief Shader asset
     */
    class PENROSE_API ShaderAsset final: public Asset {
    public:
        explicit ShaderAsset(std::shared_ptr<Shader> &&shader)
            : _shader(std::forward<decltype(shader)>(shader)) {
            //
        }

        ~ShaderAsset() override = default;

        //! \copydoc Asset::getType
        [[nodiscard]] AssetType getType() const override { return AssetType::Shader; }

        /**
         * \brief Get pointer to instance of Shader
         * \return instance of Shader
         */
        [[nodiscard]] std::weak_ptr<Shader> getShader() const { return this->_shader; }

    private:
        std::shared_ptr<Shader> _shader;
    };
}

#endif // PENROSE_ASSETS_SHADER_ASSET_HPP
