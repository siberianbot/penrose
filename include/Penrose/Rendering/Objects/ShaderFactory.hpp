#ifndef PENROSE_RENDERING_SHADER_FACTORY_HPP
#define PENROSE_RENDERING_SHADER_FACTORY_HPP

#include <vector>

#include <Penrose/Rendering/Objects/Shader.hpp>

namespace Penrose {

    /**
     * \brief Shader object factory interface
     */
    class PENROSE_API ShaderFactory {
    public:
        virtual ~ShaderFactory() = default;

        /**
         * \brief Create instance of shader object
         * \param rawData Raw shader data
         * \return Instance of shader object
         */
        [[nodiscard]] virtual Shader *makeShader(std::vector<std::byte> &&rawData) = 0;
    };
}

#endif // PENROSE_RENDERING_SHADER_FACTORY_HPP
