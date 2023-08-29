#ifndef PENROSE_RESOURCES_LAZY_HPP
#define PENROSE_RESOURCES_LAZY_HPP

#include <functional>
#include <optional>
#include <vector>

#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    template<IsResource T>
    class Lazy {
    public:
        using Factory = std::function<T *()>;

        explicit Lazy(Factory factory);

        [[nodiscard]] T *operator->();

        [[nodiscard]] T *get();

        void reset();

    private:
        Factory _factory;
        std::optional<T *> _payload;

        [[nodiscard]] T *acquire();
    };

    template<IsResource T>
    class LazyCollection {
    public:
        using Factory = std::function<std::vector<T *>()>;
        using Iterator = std::vector<T *>::iterator;
        using ConstIterator = std::vector<T *>::const_iterator;

        explicit LazyCollection(Factory factory);

        [[nodiscard]] Iterator begin();

        [[nodiscard]] Iterator end();

        void reset();

    private:
        Factory _factory;
        std::optional<std::vector<T *>> _payload;

        [[nodiscard]] std::vector<T *> &acquire();
    };
}

#include "Lazy.inl"

#endif // PENROSE_RESOURCES_LAZY_HPP
