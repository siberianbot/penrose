#ifndef PENROSE_RESOURCES_LAZY_HPP
#define PENROSE_RESOURCES_LAZY_HPP

#include <functional>
#include <optional>
#include <vector>

namespace Penrose {

    template<class T>
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

    template<class T>
    class LazyCollection {
    public:
        using Factory = std::function<std::vector<T *>()>;
        using Iterator = std::vector<T *>::iterator;

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
