#ifndef PENROSE_COMMON_DOUBLE_VECTOR_HPP
#define PENROSE_COMMON_DOUBLE_VECTOR_HPP

#include <cstdint>
#include <vector>

namespace Penrose {

    template<typename T>
    class DoubleVector {
    private:
        std::vector<T> _vectors[2];
        std::uint32_t _current;

    public:
        [[nodiscard]] std::vector<T> &front() {
            return this->_vectors[this->_current];
        }

        [[nodiscard]] std::vector<T> &back() {
            auto idx = (this->_current + 1) % 2;

            return this->_vectors[idx];
        }

        void swap() {
            this->_current = (this->_current + 1) % 2;
        }
    };
}

#endif // PENROSE_COMMON_DOUBLE_VECTOR_HPP
