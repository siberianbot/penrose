#include "VkShaderAsset.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkShaderAsset::VkShaderAsset(DeviceContext *deviceContext,
                                 vk::ShaderModule shaderModule)
            : _deviceContext(deviceContext),
              _shaderModule(shaderModule) {
        //
    }

    VkShaderAsset::~VkShaderAsset() {
        this->_deviceContext->getLogicalDevice().destroy(this->_shaderModule);
    }

    VkShaderAsset *makeVkShaderAsset(DeviceContext *deviceContext,
                                     const std::uint32_t *data,
                                     std::uint32_t size) {
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(data)
                .setCodeSize(size);
        auto shaderModule = deviceContext->getLogicalDevice().createShaderModule(createInfo);

        return new VkShaderAsset(deviceContext, shaderModule);
    }
}
