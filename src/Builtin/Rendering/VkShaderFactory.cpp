#include "VkShaderFactory.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"

#include "src/Builtin/Rendering/VkShader.hpp"

namespace Penrose {

    VkShaderFactory::VkShaderFactory(ResourceSet *resources)
            : _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    Shader *VkShaderFactory::makeShader(const std::uint32_t *data, std::uint32_t size) {
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(data)
                .setCodeSize(size);
        auto shaderModule = this->_deviceContext->getLogicalDevice().createShaderModule(createInfo);

        return new VkShader(this->_deviceContext, shaderModule);
    }
}
