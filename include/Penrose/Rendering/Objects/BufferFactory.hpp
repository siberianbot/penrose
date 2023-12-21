#ifndef PENROSE_RENDERING_OBJECTS_BUFFER_FACTORY_HPP
#define PENROSE_RENDERING_OBJECTS_BUFFER_FACTORY_HPP

#include <cstdint>

#include <Penrose/Rendering/Objects/Buffer.hpp>

namespace Penrose {

    /**
     * \brief Buffer object factory interface
     */
    class PENROSE_API BufferFactory {
    public:
        virtual ~BufferFactory() = default;

        /**
         * \brief Create instance of empty buffer object
         * \param type Type of buffer
         * \param size Size of buffer
         * \param map Map buffer to memory
         * \return Instance of buffer object
         */
        [[nodiscard]] virtual Buffer *makeBuffer(BufferType type, std::uint64_t size, bool map) = 0;

        /**
         * \brief Create instance of buffer object with data
         * \param type Type of buffer
         * \param size Size of buffer
         * \param map Map buffer to memory
         * \param data Pointer to raw data
         * \return Instance of buffer object
         */
        [[nodiscard]] virtual Buffer *makeBuffer(BufferType type, std::uint64_t size, bool map, void *data) = 0;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_BUFFER_FACTORY_HPP
