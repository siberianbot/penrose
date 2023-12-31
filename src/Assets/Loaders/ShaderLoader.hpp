#ifndef PENROSE_ASSETS_LOADERS_SHADER_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_SHADER_LOADER_HPP

#include <Penrose/Rendering/Objects/ShaderFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class ShaderLoader final: public Resource<ShaderLoader>,
                              public TypedAssetLoader {
    public:
        explicit ShaderLoader(const ResourceSet *resources);
        ~ShaderLoader() override = default;

        [[nodiscard]] AssetType getAssetType() const override { return AssetType::Shader; }

        [[nodiscard]] Asset *fromReader(AssetReader &reader) override;

    private:
        ResourceProxy<ShaderFactory> _shaderFactory;
    };
}

#endif // PENROSE_ASSETS_LOADERS_SHADER_LOADER_HPP
