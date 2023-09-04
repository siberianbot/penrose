#include "VkShader.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkShader::VkShader(VkLogicalDeviceContext *logicalDeviceContext,
                       vk::ShaderModule shaderModule)
            : _logicalDeviceContext(logicalDeviceContext),
              _shaderModule(shaderModule) {
        //
    }

    VkShader::~VkShader() {
        this->_logicalDeviceContext->getHandle().destroy(this->_shaderModule);
    }
}
