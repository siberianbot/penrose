#ifndef PENROSE_RENDERING_OBJECTS_SHADER_HPP
#define PENROSE_RENDERING_OBJECTS_SHADER_HPP

namespace Penrose {

    /**
     * \brief Shader object
     * \details Shader objects are used in rendering pipelines.
     */
    class PENROSE_API Shader {
    public:
        virtual ~Shader() = default;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_SHADER_HPP
