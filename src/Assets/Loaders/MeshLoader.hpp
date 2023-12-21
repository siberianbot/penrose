#ifndef PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP

#include <Penrose/Rendering/Objects/BufferFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class MeshLoader final: public Resource<MeshLoader>,
                            public TypedAssetLoader {
    public:
        explicit MeshLoader(const ResourceSet *resources);
        ~MeshLoader() override = default;

        [[nodiscard]] AssetType getAssetType() const override { return AssetType::Mesh; }

        [[nodiscard]] Asset *fromReader(AssetReader &reader) override;

    private:
        ResourceProxy<BufferFactory> _bufferFactory;
    };
}

#endif // PENROSE_ASSETS_LOADERS_MESH_LOADER_HPP
