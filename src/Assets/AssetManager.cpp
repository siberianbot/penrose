#include "AssetManager.hpp"

#include <fmt/core.h>
#include <vulkan/vulkan.hpp>

#include "src/Assets/AssetDictionary.hpp"
#include "src/Common/EngineError.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    AssetManager::AssetManager(ResourceSet *resources)
            : _assetDictionary(resources->get<AssetDictionary>()),
              _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    ShaderAsset AssetManager::loadShader(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(reinterpret_cast<const std::uint32_t *>(openedAsset.data()))
                .setCodeSize(openedAsset.size());

        try {
            return ShaderAsset{
                    .shaderModule = makeShaderModule(this->_deviceContext, createInfo)
            };
        } catch (...) {
            std::throw_with_nested(EngineError(fmt::format("Failed to create shader module from asset {}", asset)));
        }
    }
}
