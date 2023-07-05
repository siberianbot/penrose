#ifndef PENROSE_COMMON_LAZY_HPP
#define PENROSE_COMMON_LAZY_HPP

#include <functional>
#include <optional>

namespace Penrose {

    template<typename T>
    class Lazy {
        using Provider = std::function<T *()>;

    private:
        Provider _provider;
        std::optional<T *> _ptr;

        void acquire();

    public:
        explicit Lazy(const Provider &provider);

        [[nodiscard]] T *operator->();
    };
}

#include "Lazy.inl"

#endif // PENROSE_COMMON_LAZY_HPP
