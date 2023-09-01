#include "VkShader.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkShader::VkShader(DeviceContext *deviceContext,
                       vk::ShaderModule shaderModule)
            : _deviceContext(deviceContext),
              _shaderModule(shaderModule) {
        //
    }

    VkShader::~VkShader() {
        this->_deviceContext->getLogicalDevice().destroy(this->_shaderModule);
    }
}
