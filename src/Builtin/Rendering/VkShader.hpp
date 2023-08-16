#ifndef PENROSE_BUILTIN_RENDERING_VK_SHADER_HPP
#define PENROSE_BUILTIN_RENDERING_VK_SHADER_HPP

#include <Penrose/Rendering/Shader.hpp>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class DeviceContext;

    class VkShader : public Shader {
    public:
        VkShader(DeviceContext *deviceContext,
                 vk::ShaderModule shaderModule);
        ~VkShader() override;

        [[nodiscard]] const vk::ShaderModule &getShaderModule() const { return this->_shaderModule; }

    private:
        DeviceContext *_deviceContext;

        vk::ShaderModule _shaderModule;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_SHADER_HPP
