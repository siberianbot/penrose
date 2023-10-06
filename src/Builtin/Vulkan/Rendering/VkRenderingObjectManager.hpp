#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERING_OBJECT_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERING_OBJECT_MANAGER_HPP

#include <Penrose/Rendering/RenderingObjectManager.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkShaderFactory.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderingObjectManager : public Resource, public RenderingObjectManager {
    public:
        explicit VkRenderingObjectManager(ResourceSet *resources);
        ~VkRenderingObjectManager() override = default;

        [[nodiscard]] Buffer *makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, void *data) override;

        [[nodiscard]] Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                       std::vector<std::byte> &&rawData) override;

        [[nodiscard]] Shader *makeShader(std::vector<std::byte> &&rawData) override;

    private:
        Lazy<VkBufferFactory> _bufferFactory;
        Lazy<VkImageFactory> _imageFactory;
        Lazy<VkShaderFactory> _shaderFactory;
        Lazy<VkCommandManager> _commandManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDERING_OBJECT_MANAGER_HPP
