#include <Penrose/Assets/AssetLoader.hpp>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/AssetStructs.hpp"

namespace Penrose {

    bool compare(std::size_t size, const char *a, const char *b) {
        for (std::size_t idx = 0; idx < size; ++idx) {
            if (a[idx] != b[idx]) {
                return false;
            }
        }

        return true;
    }

    AssetLoader::AssetLoader(ResourceSet *resources)
            : _imageAssetFactory(resources->getLazy<ImageAssetFactory>()),
              _meshAssetFactory(resources->getLazy<MeshAssetFactory>()),
              _shaderAssetFactory(resources->getLazy<ShaderAssetFactory>()) {

        this->_providers[AssetType::Mesh] = [this](AssetReader &reader) {
            auto info = reader.read<MeshInfo>();

            auto vertices = std::vector<Vertex>(info.verticesCount);
            auto indices = std::vector<std::uint32_t>(info.indicesCount);

            reader.read(sizeof(Vertex) * info.verticesCount, vertices.data());
            reader.read(sizeof(std::uint32_t) * info.indicesCount, indices.data());

            return this->_meshAssetFactory->makeMesh(std::move(vertices), std::move(indices));
        };

        this->_providers[AssetType::Image] = [this](AssetReader &reader) {
            auto info = reader.read<ImageInfo>();

            auto data = std::vector<std::byte>(info.size);
            reader.read(info.size, data.data());

            return this->_imageAssetFactory->makeImage(ImageFormat::RGBA, info.width, info.height, data.data());
        };

        this->_providers[AssetType::Shader] = [this](AssetReader &reader) {
            auto info = reader.read<ShaderInfo>();

            auto data = std::vector<std::byte>(info.size);
            reader.read(info.size, data.data());

            return this->_shaderAssetFactory->makeShader(reinterpret_cast<const std::uint32_t *>(data.data()),
                                                         info.size);
        };
    }

    Asset *AssetLoader::load(const std::filesystem::path &path) const {
        auto reader = AssetReader(path);

        reader.open();

        auto magic = reader.read<MagicHeader>();
        if (!compare(4, ASSET_MAGIC, magic.value)) {
            throw EngineError("File is not a Penrose Asset");
        }

        auto version = reader.read<VersionHeader>();
        if (version.value != ASSET_VERSION) {
            throw EngineError(
                    fmt::format("Version mismatch: required v{}, v{} is not supported", ASSET_VERSION, version.value)
            );
        }

        auto header = reader.read<V1Header>();
        auto providerIt = this->_providers.find(header.type);

        if (providerIt == this->_providers.end()) {
            throw EngineError("Not supported asset type");
        }

        return providerIt->second(reader);
    }
}
