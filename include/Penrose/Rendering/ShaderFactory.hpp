#ifndef PENROSE_RENDERING_SHADER_FACTORY_HPP
#define PENROSE_RENDERING_SHADER_FACTORY_HPP

#include <cstdint>

#include <Penrose/Rendering/Shader.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ShaderFactory : public Resource {
    public:
        ~ShaderFactory() override = default;

        [[nodiscard]] virtual Shader *makeShader(const std::uint32_t *data, std::uint32_t size) = 0;
    };
}

#endif // PENROSE_RENDERING_SHADER_FACTORY_HPP
