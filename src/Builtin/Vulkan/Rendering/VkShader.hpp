#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_HPP

#include <Penrose/Rendering/Shader.hpp>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;

    class VkShader : public Shader {
    public:
        VkShader(VkLogicalDeviceContext *logicalDeviceContext,
                 vk::ShaderModule shaderModule);
        ~VkShader() override;

        [[nodiscard]] const vk::ShaderModule &getShaderModule() const { return this->_shaderModule; }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        vk::ShaderModule _shaderModule;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SHADER_HPP
