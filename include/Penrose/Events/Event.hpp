#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <type_traits>

#include <Penrose/Api.hpp>

namespace Penrose {

    template<typename Self>
    class PENROSE_API Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]] constexpr static Self create() {
            static_assert(std::is_default_constructible_v<Self>);

            return Self();
        }

        template<typename ...Args>
        [[nodiscard]] constexpr static Self create(Args &&...args) {
            static_assert(std::is_constructible_v<Self, Args...>);

            return Self(std::forward<decltype(args)>(args)...);
        }
    };
}

#endif // PENROSE_EVENTS_EVENT_HPP
