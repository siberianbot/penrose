#ifndef PENROSE_RENDERING_RENDERING_OBJECT_MANAGER_HPP
#define PENROSE_RENDERING_RENDERING_OBJECT_MANAGER_HPP

#include <cstdint>
#include <vector>

#include <Penrose/Rendering/Buffer.hpp>
#include <Penrose/Rendering/Image.hpp>
#include <Penrose/Rendering/Shader.hpp>

namespace Penrose {

    class RenderingObjectManager {
    public:
        virtual ~RenderingObjectManager() = default;

        [[nodiscard]] virtual Buffer *makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count,
                                                 void *data) = 0;

        [[nodiscard]] virtual Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                               std::vector<std::byte> &&rawData) = 0;

        [[nodiscard]] virtual Shader *makeShader(std::vector<std::byte> &&rawData) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDERING_OBJECT_MANAGER_HPP
