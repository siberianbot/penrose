#include <Penrose/Assets/AssetLoader.hpp>

#include <Penrose/Assets/ImageAsset.hpp>
#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Assets/ShaderAsset.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Common/Vertex.hpp>
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
            : _renderingObjectManager(resources->getLazy<RenderingObjectManager>()) {

        this->_providers[AssetType::Mesh] = [this](AssetReader &reader) {

            auto info = reader.read<MeshInfo>();

            auto verticesSize = sizeof(Vertex) * info.verticesCount;
            auto indicesSize = sizeof(std::uint32_t) * info.indicesCount;

            auto vertices = std::vector<Vertex>(info.verticesCount);
            auto indices = std::vector<std::uint32_t>(info.indicesCount);

            reader.read(verticesSize, vertices.data());
            reader.read(indicesSize, indices.data());

            auto vertexBuffer = this->_renderingObjectManager->makeBuffer(BufferType::Vertex,
                                                                          verticesSize,
                                                                          info.verticesCount,
                                                                          vertices.data());

            auto indexBuffer = this->_renderingObjectManager->makeBuffer(BufferType::Index,
                                                                         indicesSize,
                                                                         info.indicesCount,
                                                                         indices.data());

            return new MeshAsset(std::unique_ptr<Buffer>(vertexBuffer),
                                 std::unique_ptr<Buffer>(indexBuffer));
        };

        this->_providers[AssetType::Image] = [this](AssetReader &reader) {
            auto info = reader.read<ImageInfo>();

            auto rawData = std::vector<std::byte>(info.size);
            reader.read(info.size, rawData.data());

            auto image = this->_renderingObjectManager->makeImage(info.format, info.width, info.height,
                                                                  std::forward<decltype(rawData)>(rawData));

            return new ImageAsset(std::unique_ptr<Image>(image));
        };

        this->_providers[AssetType::Shader] = [this](AssetReader &reader) {
            auto info = reader.read<ShaderInfo>();

            auto rawData = std::vector<std::byte>(info.size);
            reader.read(info.size, rawData.data());

            auto shader = this->_renderingObjectManager->makeShader(std::forward<decltype(rawData)>(rawData));

            return new ShaderAsset(std::unique_ptr<Shader>(shader));
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
            throw EngineError("Version mismatch: required v{}, v{} is not supported", ASSET_VERSION, version.value);
        }

        auto header = reader.read<V1Header>();
        auto providerIt = this->_providers.find(header.type);

        if (providerIt == this->_providers.end()) {
            throw EngineError("Not supported asset type");
        }

        return providerIt->second(reader);
    }
}
