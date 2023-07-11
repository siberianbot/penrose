#ifndef PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_HPP
#define PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_HPP

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Assets/Asset.hpp>

namespace Penrose {

    class DeviceContext;

    class VkShaderAsset : public ShaderAsset {
    public:
        VkShaderAsset(DeviceContext *deviceContext,
                      vk::ShaderModule shaderModule);
        ~VkShaderAsset() override;

        [[nodiscard]] const vk::ShaderModule &getShaderModule() const { return this->_shaderModule; }

    private:
        DeviceContext *_deviceContext;
        vk::ShaderModule _shaderModule;
    };

    [[nodiscard]] VkShaderAsset *makeVkShaderAsset(DeviceContext *deviceContext,
                                                   const std::uint32_t *data,
                                                   std::uint32_t size);
}

#endif // PENROSE_BUILTIN_ASSETS_VK_SHADER_ASSET_HPP
