#include "ShaderLoader.hpp"

#include <Penrose/Assets/ShaderAsset.hpp>

#include "src/Assets/Structs.hpp"

namespace Penrose {

    ShaderLoader::ShaderLoader(const ResourceSet *resources)
        : _shaderFactory(resources->get<ShaderFactory>()) {
        //
    }

    Asset *ShaderLoader::fromReader(AssetReader &reader) {
        const auto [size] = reader.read<ShaderInfo>();

        auto rawData = std::vector<std::byte>(size);
        reader.read(size, rawData.data());

        const auto shader = this->_shaderFactory->makeShader(std::forward<decltype(rawData)>(rawData));

        return new ShaderAsset(std::shared_ptr<Shader>(shader));
    }
}
