#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SHADER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SHADER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/Shader.hpp>

namespace Penrose {

    class VkShader final: public Shader {
    public:
        explicit VkShader(vk::UniqueShaderModule &&shaderModule)
            : _shaderModule(std::forward<decltype(shaderModule)>(shaderModule)) {
            //
        }

        ~VkShader() override = default;

        [[nodiscard]] const vk::ShaderModule &getHandle() const { return this->_shaderModule.get(); }

    private:
        vk::UniqueShaderModule _shaderModule;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SHADER_HPP
