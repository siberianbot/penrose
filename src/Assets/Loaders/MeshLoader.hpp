#ifndef PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP

#include <Penrose/Rendering/RenderingObjectManager.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class MeshLoader final: public Resource<MeshLoader, ResourceGroup::Assets>,
                            public TypedAssetLoader {
    public:
        explicit MeshLoader(ResourceSet *resources);
        ~MeshLoader() override = default;

        [[nodiscard]] AssetType getAssetType() const override { return AssetType::Mesh; }

        [[nodiscard]] Asset *fromReader(AssetReader &reader) override;

    private:
        ResourceProxy<RenderingObjectManager> _renderingObjectManager;
    };
}

#endif // PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP
