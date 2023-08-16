#include "VkShaderAssetFactory.hpp"

#include <Penrose/Rendering/ShaderFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    VkShaderAssetFactory::VkShaderAssetFactory(ResourceSet *resources)
            : _shaderFactory(resources->get<ShaderFactory>()) {
        //
    }

    ShaderAsset *VkShaderAssetFactory::makeShader(const std::uint32_t *data, std::uint32_t size) {
        auto shader = this->_shaderFactory->makeShader(data, size);

        return new ShaderAsset(std::unique_ptr<Shader>(shader));
    }
}
