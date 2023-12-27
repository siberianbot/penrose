#ifndef PENROSE_COMMON_BIT_SET_HPP
#define PENROSE_COMMON_BIT_SET_HPP

#include <algorithm>
#include <cstdint>
#include <vector>

namespace Penrose {

    class BitSet {
    public:
        using Block = std::uint64_t;

        static constexpr std::size_t BITS_PER_BLOCK = sizeof(Block) * 8;

        explicit BitSet(const std::size_t size = BITS_PER_BLOCK)
            : _size(size) {
            this->resize(size);
        }

        [[nodiscard]] std::size_t size() const { return this->_size; }

        void set(const std::size_t idx, const bool value) {
            const auto blockIdx = idx / BITS_PER_BLOCK;
            const auto bitIdx = idx % BITS_PER_BLOCK;

            if (value) {
                this->_blocks.at(blockIdx) |= (static_cast<Block>(1) << bitIdx);
            } else {
                this->_blocks.at(blockIdx) &= ~(static_cast<Block>(1) << bitIdx);
            }
        }

        [[nodiscard]] bool at(const std::size_t idx) const {
            const auto blockIdx = idx / BITS_PER_BLOCK;
            const auto bitIdx = idx % BITS_PER_BLOCK;

            return this->_blocks.at(blockIdx) & (static_cast<Block>(1) << bitIdx);
        }

        [[nodiscard]] bool any() const {
            return std::ranges::any_of(this->_blocks, [](const auto &block) { return block != 0; });
        }

        [[nodiscard]] bool all() const {
            const auto remainder = this->_size % BITS_PER_BLOCK;

            for (std::size_t blockIdx = 0; blockIdx < this->_blocks.size(); blockIdx++) {
                Block mask = 0;

                if (blockIdx == this->_blocks.size() - 1 && remainder > 0) {
                    for (std::uint32_t bitIdx = 0; bitIdx < remainder; bitIdx++) {
                        mask |= static_cast<Block>(1) << bitIdx;
                    }
                } else {
                    mask = std::numeric_limits<Block>::max();
                }

                if (this->_blocks.at(blockIdx) ^ mask) {
                    return false;
                }
            }

            return true;
        }

        void resize(const std::size_t size) {
            auto containerSize = size / BITS_PER_BLOCK;

            if (size % BITS_PER_BLOCK > 0) {
                containerSize++;
            }

            this->_blocks.resize(containerSize, 0);

            if (this->_size < size) {
                for (auto idx = this->_size; idx < size; idx++) {
                    this->set(idx, false);
                }
            }

            this->_size = size;
        }

        void clear() {
            this->_blocks.clear();
            this->resize(this->_size);
        }

    private:
        std::size_t _size;
        std::vector<Block> _blocks;
    };
}

#endif // PENROSE_COMMON_BIT_SET_HPP
