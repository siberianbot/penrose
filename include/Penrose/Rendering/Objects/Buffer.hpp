#ifndef PENROSE_RENDERING_OBJECTS_BUFFER_HPP
#define PENROSE_RENDERING_OBJECTS_BUFFER_HPP

#include <cstdint>
#include <memory>

namespace Penrose {

    /**
     * \brief Type of buffer object
     */
    enum class BufferType {
        Uniform,
        Vertex,
        Index
    };

    /**
     * \brief Buffer object
     * \details Buffer objects represents allocated in GPU buffer for various purposes, i.e. mesh' vertex or index data.
     */
    class PENROSE_API Buffer {
    public:
        /**
         * \brief Type of pointer to raw data of buffer
         */
        using DataPtr = void *;

        virtual ~Buffer() = default;

        /**
         * \brief Get type of buffer
         * \return Type of buffer
         */
        [[nodiscard]] virtual BufferType getType() const = 0;

        /**
         * \brief Get size of buffer
         * \return Size of buffer
         */
        [[nodiscard]] virtual std::uint64_t getSize() const = 0;

        /**
         * \brief Get pointer to raw data of buffer
         * \return Pointer to raw data of buffer
         */
        [[nodiscard]] virtual DataPtr getData() const = 0;
    };

    /**
     * \brief Typed adapter over buffer object
     * \tparam T Type of data inside of buffer
     */
    template <typename T>
    class BufferAdapter {
    public:
        /**
         * \brief Construct buffer adapter from buffer pointer
         * \param buffer Pointer to buffer
         */
        explicit BufferAdapter(Buffer *buffer)
            : _buffer(buffer) {
            //
        }

        /**
         * \brief Construct buffer adapter from unique pointer to buffer
         * \param buffer Unique pointer to buffer
         */
        explicit BufferAdapter(std::unique_ptr<Buffer> &&buffer)
            : _buffer(std::forward<decltype(buffer)>(buffer)) {
            //
        }

        /**
         * \brief Get type of buffer
         * \return Type of buffer
         */
        [[nodiscard]] BufferType getType() const { return this->_buffer->getType(); }

        /**
         * \brief Get size of buffer
         * \return Size of buffer
         */
        [[nodiscard]] std::uint64_t getSize() const { return this->_buffer->getSize(); }

        /**
         * \brief Get count of items in buffer
         * \return Count of items in buffer
         */
        [[nodiscard]] std::uint32_t getCount() const { return this->_buffer->getSize() / sizeof(T); }

        /**
         * \brief Get pointer to first element of buffer
         * \return Pointer to first element of buffer
         */
        [[nodiscard]] T *begin() { return static_cast<T *>(this->_buffer->getData()); }

        /**
         * \brief Get pointer to last element of buffer
         * \return Reference to last element of buffer
         */
        [[nodiscard]] T *end() { return static_cast<T *>(this->_buffer->getData()) + this->getCount(); }

    private:
        std::unique_ptr<Buffer> _buffer;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_BUFFER_HPP
