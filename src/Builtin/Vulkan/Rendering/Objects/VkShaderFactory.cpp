#include "VkShaderFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/Objects/VkShader.hpp"

namespace Penrose {

    VkShaderFactory::VkShaderFactory(const ResourceSet *resources)
        : _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()) {
        //
    }

    Shader *VkShaderFactory::makeShader(std::vector<std::byte> &&rawData) {
        const auto createInfo = vk::ShaderModuleCreateInfo()
                                    .setPCode(reinterpret_cast<const uint32_t *>(rawData.data()))
                                    .setCodeSize(rawData.size());

        auto shaderModule = this->_logicalDeviceProvider->getLogicalDevice().handle->createShaderModuleUnique(createInfo
        );

        return new VkShader(std::move(shaderModule));
    }
}
