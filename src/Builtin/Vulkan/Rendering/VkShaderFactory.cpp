#include "VkShaderFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/VkShader.hpp"

namespace Penrose {

    VkShaderFactory::VkShaderFactory(const ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()) {
        //
    }

    Shader *VkShaderFactory::makeShader(const std::uint32_t *data, std::uint32_t size) {
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(data)
                .setCodeSize(size);
        auto shaderModule = this->_logicalDeviceContext->getHandle().createShaderModule(createInfo);

        return new VkShader(this->_logicalDeviceContext.get(), shaderModule);
    }
}
